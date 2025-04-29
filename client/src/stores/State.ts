import { writable, type Writable } from "svelte/store";

export const configModalOpen = writable(false);
export function openConfigModal() {
  configModalOpen.set(true);
}

export const rssiData = writable(0);
export const lapTimes = writable([]);
export const raceState = writable(0); // 0 ready, 1 waiting for countdown, 2 running, 3 finished, 4 false start

export const frequency = writable(5732);

export const minLapTime = persistentWritable<number>("minLapTime", 3);
export const lapNumber = persistentWritable<number>("lapNumber", 3);
export const maxRaceStartCountdown = persistentWritable<number>(
  "maxRaceStartCountdown",
  5,
);
export const enableAudioBeep = persistentWritable<boolean>(
  "enableAudioBeep",
  true,
);
export const enableAnnouncer = persistentWritable<boolean>(
  "enableAnnouncer",
  true,
);

function persistentWritable<T>(key: string, initialValue: T): Writable<T> {
  let storedValue: T = initialValue;

  if (typeof window !== "undefined") {
    const fromLocalStorage = localStorage.getItem(key);

    if (fromLocalStorage !== null) {
      try {
        storedValue = JSON.parse(fromLocalStorage) as T;
      } catch (e) {
        console.error(`Error parsing localStorage key "${key}"`, e);
      }
    }
  }

  const store = writable<T>(storedValue);

  store.subscribe((value) => {
    if (typeof window !== "undefined") {
      localStorage.setItem(key, JSON.stringify(value));
    }
  });

  return store;
}
