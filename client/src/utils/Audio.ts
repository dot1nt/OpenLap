export function playBeep() {
  if (typeof window == "undefined") {
    return;
  }

  const context = new (window.AudioContext ||
    (window as any).webkitAudioContext)();
  const oscillator = context.createOscillator();
  const gainNode = context.createGain();

  oscillator.frequency.setValueAtTime(1000, context.currentTime);

  oscillator.connect(gainNode);
  gainNode.connect(context.destination);

  oscillator.start();
  oscillator.stop(context.currentTime + 0.1);

  oscillator.onended = () => {
    context.close();
  };
}

export function playAnnouncer(text: string) {
  if (typeof window == "undefined") {
    return;
  }

  let msg = new SpeechSynthesisUtterance(text);
  msg.lang = "en-US";
  speechSynthesis.speak(msg);
}

export function playAnnouncerLap(lapNumber: number, lapTime: number) {
  const text = `Lap ${lapNumber} : ${lapTime}`;
  playAnnouncer(text);
}
