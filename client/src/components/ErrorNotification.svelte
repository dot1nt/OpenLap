<script>
  let { errorMessage, close } = $props();
</script>

{#if errorMessage !== ""}
  <div class="notification is-danger custom-notification">
    <button class="delete" aria-label="Close" onclick={() => close()}></button>
    {errorMessage}
  </div>
{/if}

<style>
  .custom-notification {
    position: fixed;
    bottom: 1rem;
    right: 1rem;
    z-index: 999;
    max-width: 50rem;
    margin-left: 1rem;
    padding-right: 3rem;
    word-break: break-word;
    overflow-wrap: break-word;
  }
</style>
