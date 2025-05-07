#include "web.h"

#include <ArduinoJson.h>
#include <AsyncTCP.h>

WebSocketHandler::WebSocketHandler() : Task("WebSocketHandler", 2048, 1) {
  m_ws.onEvent([this](auto... args) { this->onEvent(args...); });
}

AsyncWebSocket *WebSocketHandler::getSocket() { return &m_ws; }

void WebSocketHandler::run() {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(WEBSOCKET_CLEANUP_INTERVAL));
    m_ws.cleanupClients();
  }
}

void WebSocketHandler::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = static_cast<AwsFrameInfo *>(arg);
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    String message = (char *)data;

    JsonDocument json;
    DeserializationError error = deserializeJson(json, message);

    if (error) {
      return;
    }

    String command = json["command"] | "";

    if (command == "calibrate") {
      addToQueue(Command::CALIBRATE_RECEIVER);

    } else if (command == "getState") {
      addToQueue(Command::GET_STATE);

    } else if (command == "stopRace") {
      addToQueue(Command::STOP_RACE);

    } else if (command == "setFrequency") {
      if (json["value"].is<uint16_t>()) {
        uint16_t frequency = json["value"].as<uint16_t>();
        WebSocketMessage msg = FrequencyChange{frequency};
        addToQueue(msg);
      }

    } else if (command == "startRace") {
      if (json["lapNumber"].is<uint16_t>() && json["maxRaceStartCountdown"].is<uint16_t>() && json["minLapTime"].is<uint16_t>()) {
        uint16_t lapNumber = json["lapNumber"].as<uint16_t>();
        uint16_t maxRaceStartCountdown = json["maxRaceStartCountdown"].as<uint16_t>();
        uint16_t minLapTime = json["minLapTime"].as<uint16_t>();

        addToQueue(RaceStart{lapNumber, maxRaceStartCountdown, minLapTime});
      }
    }
  }
}

void WebSocketHandler::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;

    default:
      break;
  }
}

void WebSocketHandler::sendRssiData(int value) {
  JsonDocument json;
  String jsonStr;

  json["event"] = "rssi";
  json["value"] = value;

  serializeJson(json, jsonStr);
  m_ws.textAll(jsonStr);
}

void WebSocketHandler::sendBatteryData(int value) {
  JsonDocument json;
  String jsonStr;

  json["event"] = "battery";
  json["value"] = value;

  serializeJson(json, jsonStr);
  m_ws.textAll(jsonStr);
}

void WebSocketHandler::sendState(int raceState, const std::vector<unsigned long> &lapTimes, uint16_t frequency) {
  JsonDocument json;
  String jsonStr;

  json["event"] = "state";
  json["raceState"] = raceState;
  json["frequency"] = frequency;

  JsonArray lapsArray = json["lapTimes"].to<JsonArray>();
  for (int lapTime : lapTimes) {
    lapsArray.add(lapTime);
  }

  serializeJson(json, jsonStr);
  m_ws.textAll(jsonStr);
}

void WebSocketHandler::addToQueue(const WebSocketMessage &msg) {
  for (QueueHandle_t &queue : m_listeners) {
    xQueueSendToBack(queue, &msg, portMAX_DELAY);
  }
}

void WebSocketHandler::addListener(QueueHandle_t queue) {
  m_listeners.push_back(queue);
}