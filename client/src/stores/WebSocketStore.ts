import { rssiData, lapTimes, raceState, frequency } from "./State";

let socket: WebSocket;

export function connectWebSocket(url: string) {
  socket = new WebSocket(url);

  socket.onopen = () => {
    websocketGetSettings();
  };

  socket.onmessage = onMessage;

  socket.onclose = () => {
    setTimeout(() => {
      connectWebSocket(url);
    }, 100);
  };
}

function onMessage(event: MessageEvent) {
  const json = JSON.parse(event.data);

  switch (json.event) {
    case "data":
      rssiData.set(json.value);
      break;

    case "state":
      lapTimes.set(json.lapTimes);
      raceState.set(json.raceState);
      frequency.set(json.frequency);
      break;
  }
}

export function websocketGetSettings() {
  socket.send(JSON.stringify({ command: "getState" }));
}

export function websocketSendRaceStart(
  lapNumber: number,
  minLapTime: number,
  maxRaceStartCountdown: number,
) {
  const data = {
    command: "startRace",
    lapNumber: lapNumber,
    maxRaceStartCountdown: maxRaceStartCountdown,
    minLapTime: minLapTime,
  };

  socket.send(JSON.stringify(data));
}

export function websocketSendRaceStop() {
  socket.send(JSON.stringify({ command: "stopRace" }));
}

export function websocketSaveFrequency(newFrequency: number) {
  socket.send(JSON.stringify({ command: "setFrequency", value: newFrequency }));
}

export function websocketCalibrateThreshold() {
  socket.send(JSON.stringify({ command: "calibrate" }));
}
