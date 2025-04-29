#include "peak_detector.h"

std::optional<PeakDetector::TimeValue> PeakDetector::addData(int data, unsigned long time) {
  if (data > m_threshold) {
    if (data > peak.value) {
      peak = {data, time};
    }

    m_isAboveThreshold = true;

  } else if (m_isAboveThreshold) {
    m_isAboveThreshold = false;
    peak = {0, time};

    return peak;
  }

  return std::nullopt;
}

void PeakDetector::setThreshold(int threshold) {
  m_threshold = threshold;
}