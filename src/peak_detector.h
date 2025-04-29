#pragma once

#include <Arduino.h>

class PeakDetector {
 public:
  struct TimeValue {
    int value;
    unsigned long time;
  };

  PeakDetector() = default;

  std::optional<PeakDetector::TimeValue> addData(int data, unsigned long time);
  void setThreshold(int threshold);

 private:
  TimeValue peak;

  bool m_isAboveThreshold = false;
  int m_threshold = 1000;
};