<script lang="ts">
  import {
    lapTimes,
    enableAnnouncer,
    enableAudioBeep,
    raceState,
  } from "../stores/State";
  import { playBeep, playAnnouncerLap } from "../utils/Audio";
  import { get } from "svelte/store";

  import RaceTimer from "./RaceTimer.svelte";

  let laps: number[] = [];
  let fastestLapTime = Infinity;
  let fastestLapIndex = -1;

  function round(num: number): number {
    return Math.round(num / 10) / 100;
  }

  raceState.subscribe((state) => {
    if (state == 1) {
      laps = [];
    }

    if (get(enableAudioBeep) && state == 2) {
      playBeep();
    }
  });

  lapTimes.subscribe((times) => {
    if ($raceState != 2) {
      return;
    }

    const newLaps = [];
    let newFastestLapTime = Infinity;
    let newFastestLapIndex = -1;

    for (let i = 1; i < times.length; i++) {
      let time = round(times[i] - times[i - 1]);
      newLaps.push(time);

      if (time < newFastestLapTime && i != 1) {
        newFastestLapTime = time;
        newFastestLapIndex = i - 1;
      }
    }

    laps = newLaps;
    fastestLapTime = newFastestLapTime;
    fastestLapIndex = newFastestLapIndex;

    if (get(enableAudioBeep) && newLaps.length != 0) {
      playBeep();
    }

    if (get(enableAnnouncer) && newLaps.length != 0) {
      playAnnouncerLap(newLaps.length - 1, newLaps[newLaps.length - 1]);
    }
  });
</script>

<div class="box" style="margin-top: 1rem;">
  <div class="columns is-vcentered is-mobile">
    <div class="column">
      <h2 class="subtitle">Laps</h2>
    </div>
    <div class="column has-text-right">
      <RaceTimer />
    </div>
  </div>
  <table class="table is-fullwidth is-striped">
    <thead>
      <tr>
        <th>Lap Number</th>
        <th class="is-narrow">Time (Seconds)</th>
      </tr>
    </thead>
    <tbody>
      {#each laps as lap, index}
        <tr>
          {#if index === 0}
            <td>---</td>
          {:else}
            <td>{index}</td>
          {/if}
          <td>
            {lap}
            {#if index === fastestLapIndex}
              <span>🚀</span>
            {:else if index !== 0}
              <small
                class={lap - fastestLapTime > 0
                  ? "has-text-danger"
                  : "has-text-success"}
              >
                +({Math.round((lap - fastestLapTime) * 1000) / 1000})
              </small>
            {/if}
          </td>
        </tr>
      {/each}
    </tbody>
  </table>
</div>
