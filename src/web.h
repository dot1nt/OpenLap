#pragma once

#include <ESPAsyncWebServer.h>

#include <variant>
#include <vector>

#include "task.h"

class WebSocketHandler : public Task<WebSocketHandler> {
 public:
  enum class Command {
    CALIBRATE_RECEIVER,
    GET_STATE,
    STOP_RACE,
  };

  struct FrequencyChange {
    uint16_t frequency;
  };

  struct RaceStart {
    uint16_t lapNumber;
    uint16_t maxRaceStartCountdown;
    uint16_t minLapTime;
  };

  using WebSocketMessage = std::variant<Command, FrequencyChange, RaceStart>;

  WebSocketHandler();

  AsyncWebSocket *getSocket();

  void run() override;

  void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

  void sendRssiData(int value);
  void sendState(int raceState, const std::vector<unsigned long> &lapTimes, uint16_t frequency);

  void addListener(QueueHandle_t queue);

 private:
  static constexpr uint32_t WEBSOCKET_CLEANUP_INTERVAL = 1000;

  void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
  void addToQueue(const WebSocketMessage &msg);

  std::vector<QueueHandle_t> m_listeners;

  AsyncWebSocket m_ws = AsyncWebSocket("/ws");
};