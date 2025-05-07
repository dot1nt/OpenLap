<script lang="ts">
  import Notification from "./Notification.svelte";
  import { batteryVoltage, minBatteryVoltage } from "../stores/State";

  let message = "";
  let messageShown = false;

  batteryVoltage.subscribe((voltage) => {
    let voltageRounded = Math.round(voltage / 10) / 100;

    if (
      $minBatteryVoltage > voltageRounded &&
      voltageRounded !== 0 &&
      !messageShown
    ) {
      message = `Battery Voltage Low: ${voltageRounded} V`;
      messageShown = true;
    }
  });
</script>

<Notification {message} close={() => (message = "")} type="warning" />
