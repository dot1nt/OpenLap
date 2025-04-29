#pragma once

#include "task.h"

class RaceManager : public Task<RaceManager> {
 public:
  RaceManager();

  using Callback = std::function<void()>;

  void onRaceStart(Callback onRaceStart);
  void onRaceEnd(Callback onRaceEnd);
  void onRaceFalseStart(Callback onRaceEnd);

  void startRace(uint16_t lapNumber, uint16_t maxRaceStartCountdown, uint16_t minLapTime);
  void stopRace();

  bool addLap(unsigned long time);

  int getRaceState();
  const std::vector<unsigned long> &getLapTimes();

  void run() override;

 private:
  enum RaceState {
    READY,
    WAITING_FOR_COUNTDOWN,
    RUNNING,
    FINISHED,
    FALSE_START,
  };

  Callback m_onRaceStart = nullptr;
  Callback m_onRaceEnd = nullptr;
  Callback m_onRaceFalseStart = nullptr;

  RaceState m_raceState = RaceState::READY;

  std::vector<unsigned long> m_lapTimes;

  unsigned long m_countdownStartTime = 0;
  unsigned long m_raceStartTime = 0;
  uint16_t m_lapNumber = 3;
  uint16_t m_minLapTime = 3;
};