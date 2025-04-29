<script>
  import {
    raceState,
    lapNumber,
    minLapTime,
    maxRaceStartCountdown,
    enableAnnouncer,
    lapTimes,
  } from "../stores/State.js";
  import {
    websocketSendRaceStart,
    websocketSendRaceStop,
  } from "../stores/WebSocketStore.js";
  import { playAnnouncer } from "../utils/Audio";

  let active = false;

  raceState.subscribe((state) => {
    active = state == 2 || state == 1;
  });

  function onClick() {
    if ($raceState !== 2 || !active) {
      active = true;
      if ($enableAnnouncer) {
        playAnnouncer(`Starting race in less than ${$maxRaceStartCountdown}`);

        setTimeout(() => {
          websocketSendRaceStart(
            $lapNumber,
            $minLapTime,
            $maxRaceStartCountdown,
          );
        }, 2 * 1000);
      } else {
        websocketSendRaceStart($lapNumber, $minLapTime, $maxRaceStartCountdown);
      }
    } else {
      active = false;
      websocketSendRaceStop();
    }
  }
</script>

<button class="button" on:click={onClick}>
  {active ? "Stop" : "Start"}
</button>
