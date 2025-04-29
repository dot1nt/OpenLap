#include "race_manager.h"

RaceManager::RaceManager() : Task("Race Manager", 2048, 2) {}

void RaceManager::onRaceStart(Callback cb) {
  m_onRaceStart = cb;
}

void RaceManager::onRaceEnd(Callback cb) {
  m_onRaceEnd = cb;
}

void RaceManager::onRaceFalseStart(Callback cb) {
  m_onRaceFalseStart = cb;
}

void RaceManager::startRace(uint16_t lapNumber, uint16_t maxRaceStartCountdown, uint16_t minLapTime) {
  m_minLapTime = minLapTime;
  m_lapNumber = lapNumber;
  m_countdownStartTime = millis();
  m_raceStartTime = m_countdownStartTime + random(0, maxRaceStartCountdown * 1000);
  m_lapTimes.clear();
  m_raceState = RaceState::WAITING_FOR_COUNTDOWN;
}

void RaceManager::stopRace() {
  m_raceState = RaceState::READY;
  if (m_onRaceEnd) {
    m_onRaceEnd();
  }
}

bool RaceManager::addLap(unsigned long time) {
  if (m_raceState != RaceState::RUNNING && m_raceState != RaceState::WAITING_FOR_COUNTDOWN) {
    return false;
  }

  if (m_lapTimes.size() > 1) {
    if (m_minLapTime * 1000 > time - m_lapTimes.back()) {
      return false;
    }
  }

  if (m_raceState == RaceState::WAITING_FOR_COUNTDOWN) {
    m_raceState = RaceState::FALSE_START;
    if (m_onRaceFalseStart) {
      m_onRaceFalseStart();
    }
    return false;
  }

  m_lapTimes.push_back(time);

  if (m_lapTimes.size() - 1 > m_lapNumber && m_lapNumber != 0) {
    m_raceState = RaceState::FINISHED;
    if (m_onRaceEnd) {
      m_onRaceEnd();
    }
    return false;
  }

  return true;
}

int RaceManager::getRaceState() {
  return static_cast<int>(m_raceState);
}

const std::vector<unsigned long> &RaceManager::getLapTimes() {
  return m_lapTimes;
}

void RaceManager::run() {
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(10));

    switch (m_raceState) {
      case RaceState::WAITING_FOR_COUNTDOWN: {
        if (millis() > m_raceStartTime) {
          m_raceState = RaceState::RUNNING;
          addLap(m_raceStartTime);
          if (m_onRaceStart) {
            m_onRaceStart();
          }
        }

        break;
      }

      case RaceState::READY:
      case RaceState::RUNNING:
      case RaceState::FINISHED:
      case RaceState::FALSE_START:
        break;
    }
  }
}