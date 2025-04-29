<script lang="ts">
  import {
    websocketSaveFrequency,
    websocketCalibrateThreshold,
  } from "../stores/WebSocketStore.js";
  import {
    configModalOpen,
    frequency,
    minLapTime,
    lapNumber,
    maxRaceStartCountdown,
    enableAudioBeep,
    enableAnnouncer,
    rssiData,
  } from "../stores/State.js";

  const freqLookup = [
    [5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725],
    [5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866],
    [5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945],
    [5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880],
    [5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917],
    [5362, 5399, 5436, 5473, 5510, 5547, 5584, 5621],
  ];

  const bandOptions = ["A", "B", "E", "Fatshark", "Raceband"];
  const channelOptions = ["1", "2", "3", "4", "5", "6", "7", "8"];

  let band = 0;
  let channel = 0;

  let rssi = 0;

  let saveButtonText = "Save";
  let calibrating = false;

  function onFrequencySaveButton() {
    websocketSaveFrequency($frequency);

    saveButtonText = "Saved";
    setTimeout(() => {
      saveButtonText = "Save";
    }, 2000);
  }

  function onCalibrateButton() {
    websocketCalibrateThreshold();

    calibrating = true;
    setTimeout(() => {
      calibrating = false;
    }, 500);
  }

  function onCloseButton() {
    configModalOpen.set(false);
  }

  function getBandAndChannel(freq: number): [number, number] {
    for (let bandIndex = 0; bandIndex < freqLookup.length; bandIndex++) {
      let band = freqLookup[bandIndex];
      for (let channelIndex = 0; channelIndex < band.length; channelIndex++) {
        if (band[channelIndex] === freq) {
          return [bandIndex, channelIndex];
        }
      }
    }

    return [0, 0];
  }

  $: frequency.set(freqLookup[band][channel]);

  frequency.subscribe((freq) => {
    [band, channel] = getBandAndChannel(freq);
  });

  $: rssi = $rssiData;
</script>

<div id="configModal" class="modal {$configModalOpen ? 'is-active' : ''}">
  <div class="modal-background"></div>
  <div class="modal-card">
    <header class="modal-card-head">
      <p class="modal-card-title">Configuration</p>
    </header>
    <section class="modal-card-body">
      <div class="columns is-vcentered is-mobile">
        <div class="column is-narrow">
          <button
            id="calibrateButton"
            class="button {calibrating ? 'is-loading' : ''}"
            on:click={onCalibrateButton}>Calibrate</button
          >
        </div>
        <div class="column has-text-right">
          <span id="calibrationRssiText">RSSI: {rssi}</span>
        </div>
      </div>
      <hr class="my-5" />
      <div class="columns is-mobile">
        <div class="field column is-narrow">
          <label for="bandSelect" class="label">Band</label>
          <div class="control">
            <div class="select">
              <select id="bandSelect" bind:value={band}>
                {#each bandOptions as band, index}
                  <option value={index}>{band}</option>
                {/each}
              </select>
            </div>
          </div>
        </div>
        <div class="field column">
          <label for="channelSelect" class="label">Channel</label>
          <div class="control">
            <div class="select">
              <select id="channelSelect" bind:value={channel}>
                {#each channelOptions as band, index}
                  <option value={index}>{band}</option>
                {/each}
              </select>
            </div>
          </div>
        </div>
        <div class="field column is-narrow is-flex is-align-items-center">
          <button
            id="saveButton"
            class="button is-link"
            on:click={onFrequencySaveButton}>{saveButtonText}</button
          >
        </div>
      </div>
      <hr class="my-5" />
      <div class="columns is-vcentered">
        <div class="column">
          <label for="minLapTimeSlider" class="label"
            >Minimum Lap Time (Seconds)</label
          >
          <input
            id="minLapTimeSlider"
            class="input spinner-input"
            type="number"
            step="1"
            min="0"
            max="100"
            bind:value={$minLapTime}
          />
        </div>
      </div>
      <div class="columns is-vcentered">
        <div class="column">
          <label for="lapNumberSlider" class="label">Lap Number</label>
          <input
            id="lapNumberSlider"
            class="input spinner-input"
            type="number"
            step="1"
            min="0"
            max="100"
            bind:value={$lapNumber}
          />
        </div>
      </div>
      <div class="columns is-vcentered">
        <div class="column">
          <label for="minLapTimeSlider" class="label"
            >Start Countdown (Seconds)</label
          >
          <input
            id="minLapTimeSlider"
            class="input spinner-input"
            type="number"
            step="1"
            min="0"
            max="100"
            bind:value={$maxRaceStartCountdown}
          />
        </div>
      </div>
      <hr class="my-5" />
      <div class="field">
        <label class="checkbox">
          <input
            id="audioBeepToggle"
            type="checkbox"
            bind:checked={$enableAudioBeep}
          />
          Audio Beep
        </label>
      </div>
      <div class="field">
        <label class="checkbox">
          <input
            id="announcerToggle"
            type="checkbox"
            bind:checked={$enableAnnouncer}
          />
          Announcer
        </label>
      </div>
    </section>
    <footer class="modal-card-foot">
      <button class="button" on:click={onCloseButton}>Close</button>
    </footer>
  </div>
</div>
