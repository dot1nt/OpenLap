<script>
  import { onMount } from "svelte";

  import StartRaceButton from "../components/StartRaceButton.svelte";
  import ConfigModal from "../components/ConfigModal.svelte";
  import LapTable from "../components/LapTable.svelte";
  import RaceEndModal from "../components/RaceEndModal.svelte";
  import FalseStartModal from "../components/RaceFalseStartModal.svelte";
  import Notification from "../components/Notification.svelte";
  import BatteryVoltageWarning from "../components/BatteryVoltageWarning.svelte";

  import { openConfigModal } from "../stores/State.js";
  import { connectWebSocket } from "../stores/WebSocketStore.js";

  let currentErrorMessage = "";

  if (typeof window !== "undefined") {
    window.onerror = (message) => {
      currentErrorMessage = `${message}`;
    };
  }

  onMount(() => {
    connectWebSocket(`ws://${window.location.hostname}/ws`);
  });
</script>

<svelte:head>
  <title>OpenLap</title>
</svelte:head>

<section class="section" style="padding-top: 1rem; padding-bottom: 1rem;">
  <div class="container">
    <div class="columns is-mobile is-variable is-1">
      <div class="column has-text-left">
        <div class="buttons" style="display: flex; align-items: center;">
          <StartRaceButton />
        </div>
      </div>
      <div class="column has-text-right is-narrow">
        <button id="openModal" class="button" on:click={openConfigModal}
          >Settings</button
        >
      </div>
    </div>
  </div>
</section>

<section class="section" style="padding-top: 0.5rem; padding-bottom: 0.5rem;">
  <div class="container">
    <LapTable />
  </div>
</section>

<ConfigModal />
<RaceEndModal />
<FalseStartModal />

<Notification
  message={currentErrorMessage}
  close={() => (currentErrorMessage = "")}
  type="danger"
/>

<BatteryVoltageWarning />
