<script lang="ts">
  import { raceState, lapTimes } from "../stores/State.js";
  import { playAnnouncer } from "../utils/Audio.js";

  let open = false;
  let raceTime = 0;
  let avgLapTime = 0;
  let minLapTime = 0;
  let maxLapTime = 0;

  function onCloseButton() {
    open = false;
  }

  function round(num: number): number {
    return Math.round(num / 10) / 100;
  }

  raceState.subscribe((state) => {
    if (state == 3) {
      playAnnouncer("Race finished");

      const laps = $lapTimes;

      if (laps.length > 2) {
        raceTime = laps[laps.length - 1] - laps[0];

        const lapDurations = [];
        for (let i = 2; i < laps.length; i++) {
          lapDurations.push(laps[i] - laps[i - 1]);
        }

        avgLapTime =
          lapDurations.reduce((a, b) => a + b, 0) / lapDurations.length;
        minLapTime = Math.min(...lapDurations);
        maxLapTime = Math.max(...lapDurations);
      } else {
        raceTime = 0;
        avgLapTime = 0;
        minLapTime = 0;
        maxLapTime = 0;
      }

      open = true;
    }
  });
</script>

<div class="modal {open ? 'is-active' : ''}" id="raceEndModal">
  <div class="modal-background"></div>
  <div class="modal-card">
    <header class="modal-card-head">
      <p class="modal-card-title">🏁 Race End</p>
    </header>
    <section class="modal-card-body">
      <div class="content">
        <p>
          <strong>🏁 Race Time:</strong> <span>{round(raceTime)}</span> seconds
        </p>
        <p>
          <strong>⏱️ Average Lap:</strong> <span>{round(avgLapTime)}</span> seconds
        </p>
        <p>
          <strong>🚀 Fastest Lap:</strong> <span>{round(minLapTime)}</span> seconds
        </p>
        <p>
          <strong>🐢 Slowest Lap:</strong> <span>{round(maxLapTime)}</span> seconds
        </p>
      </div>
    </section>
    <footer class="modal-card-foot">
      <button class="button" on:click={onCloseButton}>Close</button>
    </footer>
  </div>
</div>
