<script>
  import { raceState } from "../stores/State.js";
  import { playAnnouncer } from "../utils/Audio.js";

  let open = false;

  raceState.subscribe((state) => {
    if (state == 4) {
      playAnnouncer("False start");
      open = true;
    }
  });

  function onCloseButton() {
    open = false;
  }
</script>

<div class="modal {open ? 'is-active' : ''}" id="raceEndModal">
  <div class="modal-background"></div>
  <div class="modal-card">
    <header class="modal-card-head">
      <p class="modal-card-title">🚫 False Start</p>
    </header>
    <section class="modal-card-body">
      <div class="content"></div>
    </section>
    <footer class="modal-card-foot">
      <button class="button" on:click={onCloseButton}>Close</button>
    </footer>
  </div>
</div>
