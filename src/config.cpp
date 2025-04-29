#include "config.h"

#include <EEPROM.h>

void Config::setFrequency(uint16_t frequency) {
  m_userConfig.frequency = frequency;
  saveEEPROMConfig();
}

uint16_t Config::getFrequency() {
  return m_userConfig.frequency;
}

void Config::setThreshold(int threshold) {
  m_userConfig.threshold = threshold;
  saveEEPROMConfig();
}

int Config::getThreshold() {
  return m_userConfig.threshold;
}

void Config::loadEEPROMConfig() {
  EEPROM.begin(512);
  EEPROM.get(0, m_userConfig);

  if (!frequencyValid(m_userConfig.frequency) || !thresholdValid(m_userConfig.threshold)) {
    m_userConfig.frequency = DEFAULT_FREQUENCY;
    m_userConfig.threshold = DEFAULT_THRESHOLD;

    saveEEPROMConfig();
  }
}

bool Config::frequencyValid(uint16_t frequency) {
  return frequency < 5945 && frequency > 5645;
}

bool Config::thresholdValid(int threshold) {
  return threshold > 0 && threshold < 5000;
}

void Config::saveEEPROMConfig() {
  EEPROM.begin(512);

  EEPROM.put(0, m_userConfig);
  EEPROM.commit();
}