<script lang="ts">
  import { raceState, lapTimes } from "../stores/State.js";
  import { playAnnouncer } from "../utils/Audio.js";

  let open = false;
  let raceTime = 0;
  let avgLapTime = 0;
  let minLapTime = 0;
  let maxLapTime = 0;
  let best3Consec = 0;
  let consistency = 0;

  function onCloseButton() {
    open = false;
  }

  function round(num: number): number {
    return Math.round(num / 10) / 100;
  }

  raceState.subscribe((state) => {
    if (state == 3 && $lapTimes.length > 2) {
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

        const variance = lapDurations.reduce((sum, t) => sum + (t - avgLapTime) ** 2, 0) / lapDurations.length;
        const stdDev = Math.sqrt(variance);
        consistency = Math.round((1 - stdDev / avgLapTime) * 100);

        if (lapDurations.length >= 3) {
          best3Consec = Infinity;
          for (let i = 0; i <= lapDurations.length - 3; i++) {
            const sum = lapDurations[i] + lapDurations[i + 1] + lapDurations[i + 2];
            if (sum < best3Consec) best3Consec = sum;
          }
        } else {
          best3Consec = 0;
        }
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
          <strong>🏁 Race Time:</strong> {round(raceTime)} seconds
        </p>
        <p>
          <strong>⏱️ Average Lap:</strong> {round(avgLapTime)} seconds
        </p>
        <p>
          <strong>🎯 Consistency:</strong> {consistency}%
        </p>
        <p>
          <strong>🚀 Fastest Lap:</strong> {round(minLapTime)} seconds
        </p>
        <p>
          <strong>🐢 Slowest Lap:</strong> {round(maxLapTime)} seconds
        </p>
        {#if best3Consec !== 0}
          <p>
            <strong>🏆 Fastest 3-Consecutive Laps:</strong> <span>{round(best3Consec)}</span> seconds
          </p>
        {/if}
      </div>
    </section>
    <footer class="modal-card-foot">
      <button class="button" on:click={onCloseButton}>Close</button>
    </footer>
  </div>
</div>
