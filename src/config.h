#pragma once

#include <Arduino.h>

#include <memory>

class Config {
 public:
  // WiFi network credentials
  static constexpr const char *SSID = "Laptimer";
  static constexpr const char *WIFI_PASSWORD = "Laptimer";

  static constexpr uint8_t RSSI_PIN = 33;  // RSSI output pin
  static constexpr uint8_t CH2_PIN = 22;   // Chip select pin (CH2 on the rx5808)

  static constexpr bool ENABLE_BATTERY_MONITORING = false;       // Set this to true to enable sending battery info to the client
  static constexpr uint8_t BATTERY_PIN = 35;                     // Input pin for reading battery voltage
  static constexpr int R1 = 100000;                              // Resistor 1 (pin to battery) resistance in ohm
  static constexpr int R2 = 20000;                               // Resistor 2 (pin to GND) resistance in ohm
  static constexpr unsigned long BATTERY_UPDATE_INTERVAL = 1000; // Interval to send battery voltage to clients in milliseconds

  static constexpr unsigned long RX_UPDATE_INTERVAL = 5;      // Receiver polling interval in milliseconds
  static constexpr unsigned long RSSI_UPDATE_INTERVAL = 200;  // Interval to send RSSI data to clients in milliseconds

  static constexpr size_t RX_FILTER_SIZE = 10;  // Number of samples to average for smoothing RSSI readings

  static constexpr size_t RX_CALIBRATION_SAMPLES = 10;          // Number of samples to average during receiver calibration
  static constexpr unsigned long RX_CALIBRATION_INTERVAL = 10;  // Delay between calibration samples in milliseconds

  Config() = default;

  void setFrequency(uint16_t frequency);
  uint16_t getFrequency();

  void setThreshold(int threshold);
  int getThreshold();

  void loadEEPROMConfig();

 private:
  static constexpr uint16_t DEFAULT_FREQUENCY = 5865;  // A1
  static constexpr int DEFAULT_THRESHOLD = 1000;

  struct UserConfig {
    uint16_t frequency = DEFAULT_FREQUENCY;
    int threshold = DEFAULT_THRESHOLD;
  };

  UserConfig m_userConfig;

  void saveEEPROMConfig();

  static bool frequencyValid(uint16_t frequency);
  static bool thresholdValid(int threshold);
};