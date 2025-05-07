<script lang="ts">
  let {
    message = "",
    type = "info",
    close,
  }: {
    message: string;
    type: "info" | "warning" | "danger";
    close: () => void;
  } = $props();
</script>

{#if message}
  <div class={`notification is-${type} custom-notification`}>
    <button class="delete" aria-label="Close" onclick={close}></button>
    {message}
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
