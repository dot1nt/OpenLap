<script lang="ts">
  import { raceState } from "../stores/State.js";
  import { onDestroy } from "svelte";

  let startTime: DOMHighResTimeStamp;
  let interval: number | null = null;
  let timeFormatted: string;

  function formatTime(seconds: number): string {
    const ms = Math.floor((seconds % 1) * 100);
    const secs = Math.floor(seconds);
    return `${secs}:${ms}`;
  }

  raceState.subscribe((state) => {
    if (state == 2) {
      startTime = performance.now();

      if (!interval) {
        interval = setInterval(() => {
          timeFormatted = formatTime((performance.now() - startTime) / 1000);
        }, 10);
      }
    } else {
      if (interval) {
        clearInterval(interval);
        interval = null;
      }
    }
  });

  onDestroy(() => {
    if (interval) {
      clearInterval(interval);
    }
  });
</script>

<h2 class="has-text-weight-bold">{timeFormatted}</h2>
