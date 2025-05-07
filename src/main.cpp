#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "config.h"
#include "filter.h"
#include "peak_detector.h"
#include "race_manager.h"
#include "rx5808.h"
#include "web.h"

Config config;
Rx5808 rx(Config::RSSI_PIN, Config::CH2_PIN);
PeakDetector peakDetector;
Filter filter(Config::RX_FILTER_SIZE);
RaceManager raceManager;

AsyncWebServer server(80);
WebSocketHandler webSocketHandler;
QueueHandle_t xWebSocketMessageQueue;
unsigned long lastRssiWebSocketMessage;
unsigned long lastBatteryWebSocketMessage;

void calibrateReceiver() {
  int sum = 0;

  for (int i = 0; i < Config::RX_CALIBRATION_SAMPLES; i++) {
    sum += rx.readRssi();
    delay(Config::RX_CALIBRATION_INTERVAL);
  }

  int average = sum / Config::RX_CALIBRATION_SAMPLES;

  peakDetector.setThreshold(average);
  config.setThreshold(average);
}

void setReceiverFrequency(uint16_t frequency) {
  rx.setFrequency(frequency);
  config.setFrequency(frequency);
}

void sendStateToWebsocket() {
  int raceState = raceManager.getRaceState();
  const std::vector<unsigned long> &lapTimes = raceManager.getLapTimes();
  uint16_t frequency = config.getFrequency();

  webSocketHandler.sendState(raceState, lapTimes, frequency);
}

void vTask(void *pvParameters) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(Config::RX_UPDATE_INTERVAL));

    int rssi = rx.readRssi();
    int filteredRssi = filter.process(rssi);

    if (millis() - lastRssiWebSocketMessage > Config::RSSI_UPDATE_INTERVAL) {
      lastRssiWebSocketMessage = millis();
      webSocketHandler.sendRssiData(filteredRssi);
    }

    if (millis() - lastBatteryWebSocketMessage > Config::BATTERY_UPDATE_INTERVAL && Config::ENABLE_BATTERY_MONITORING) {
      lastBatteryWebSocketMessage = millis();
      int batteryVoltage = analogRead(Config::BATTERY_PIN);
      int batteryVoltageScaled = batteryVoltage * 3.3 / 4095 * (Config::R1 + Config::R2) / Config::R2 * 1000;

      webSocketHandler.sendBatteryData(batteryVoltageScaled);
    }

    std::optional<PeakDetector::TimeValue> peak = peakDetector.addData(filteredRssi, millis());

    if (peak.has_value()) {
      PeakDetector::TimeValue peakData = peak.value();

      if (raceManager.addLap(peakData.time)) {
        sendStateToWebsocket();
      }
    }
  }
}

void vHandleWebSocketMessages(void *pvParameters) {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(Config::RX_UPDATE_INTERVAL));

    WebSocketHandler::WebSocketMessage msg;
    if (xQueueReceive(xWebSocketMessageQueue, &msg, portMAX_DELAY) == pdTRUE) {
      if (std::holds_alternative<WebSocketHandler::Command>(msg)) {
        WebSocketHandler::Command command = std::get<WebSocketHandler::Command>(msg);

        switch (command) {
          case WebSocketHandler::Command::CALIBRATE_RECEIVER:
            calibrateReceiver();
            break;

          case WebSocketHandler::Command::GET_STATE:
            sendStateToWebsocket();
            break;

          case WebSocketHandler::Command::STOP_RACE:
            raceManager.stopRace();
            sendStateToWebsocket();
            break;

          default:
            break;
        }

      } else if (std::holds_alternative<WebSocketHandler::FrequencyChange>(msg)) {
        uint16_t frequency = std::get<WebSocketHandler::FrequencyChange>(msg).frequency;
        setReceiverFrequency(frequency);
        sendStateToWebsocket();

      } else if (std::holds_alternative<WebSocketHandler::RaceStart>(msg)) {
        WebSocketHandler::RaceStart raceInfo = std::get<WebSocketHandler::RaceStart>(msg);
        raceManager.startRace(raceInfo.lapNumber, raceInfo.maxRaceStartCountdown, raceInfo.minLapTime);
        sendStateToWebsocket();
      }
    }
  }
}

void setupServer() {
  if (!WiFi.softAP(Config::SSID, Config::WIFI_PASSWORD)) {
    Serial.println("Failed to start WiFi AP");
    return;
  }

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.serveStatic("/", LittleFS, "/");

  server.addHandler(webSocketHandler.getSocket());

  server.begin();
}

void setup() {
  Serial.begin(115200);

  config.loadEEPROMConfig();

  setupServer();

  xWebSocketMessageQueue = xQueueCreate(10, sizeof(WebSocketHandler::WebSocketMessage));
  webSocketHandler.addListener(xWebSocketMessageQueue);
  webSocketHandler.begin();

  rx.init();
  rx.setFrequency(config.getFrequency());

  peakDetector.setThreshold(config.getThreshold());

  raceManager.onRaceStart(sendStateToWebsocket);
  raceManager.onRaceEnd(sendStateToWebsocket);
  raceManager.onRaceFalseStart(sendStateToWebsocket);
  raceManager.begin();

  xTaskCreate(vHandleWebSocketMessages, "WebSocketMessages", 2048, nullptr, 2, nullptr);
  xTaskCreate(vTask, "vTask", 4096, nullptr, 2, nullptr);
}

void loop() {}