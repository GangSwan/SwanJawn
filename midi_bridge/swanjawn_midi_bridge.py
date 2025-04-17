import serial
import time
import pygame.midi
import random

# === CONFIG ===
COM_PORT = 'COM4'
BAUD_RATE = 115200
CHORD_SWITCH_INTERVAL = random.uniform(4, 12)
PACKET_SKIP_CHANCE = 0.6
OCTAVE_SPAN = 5
BASE_NOTE = 36  # C3 — nice and low

# === CHORD SHAPES ===
CHORDS = {
    "major": [0, 4, 7],
    "minor": [0, 3, 7],
    "maj7": [0, 4, 7, 11],
    "min7": [0, 3, 7, 10],
    "sus2": [0, 2, 7],
    "sus4": [0, 5, 7],
    "add9": [0, 2, 4, 7]
}

current_chord_name = random.choice(list(CHORDS.keys()))
last_chord_switch = time.time()
arp_index = 0

# === INIT MIDI ===
pygame.midi.init()
player = pygame.midi.Output(0)
INSTRUMENT = 12  # Pad 2 (Warm)
player.set_instrument(INSTRUMENT)
print("🎧 MIDI output ready:", pygame.midi.get_device_info(0))

# === INIT SERIAL ===
ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
print(f"🔌 Connected to {COM_PORT} @ {BAUD_RATE} baud")

try:
    while True:
        if time.time() - last_chord_switch > CHORD_SWITCH_INTERVAL:
            current_chord_name = random.choice(list(CHORDS.keys()))
            CHORD_SWITCH_INTERVAL = random.uniform(4, 12)
            last_chord_switch = time.time()
            arp_index = 0
            print(f"🔄 New chord shape: {current_chord_name}")

        try:
            line = ser.readline().decode().strip()
            if not line or not line[0].isdigit():
                continue

            parts = line.split(',')
            if len(parts) != 3:
                continue

            millis, _, duration = map(int, parts)

            if random.random() < PACKET_SKIP_CHANCE:
                continue

            chord = CHORDS[current_chord_name]
            full_arpeggio = [
                BASE_NOTE + interval + 12 * octave
                for octave in range(OCTAVE_SPAN)
                for interval in chord
            ]

            pitch = full_arpeggio[arp_index % len(full_arpeggio)]
            velocity = random.randint(50, 90)

            print(f"🌫️ Reverb Note: {pitch} ({current_chord_name})")

            player.note_on(pitch, velocity)
            time.sleep(duration / 1000.0 * 1.5)  # longer duration for reverb feel
            player.note_off(pitch, velocity)

            arp_index += 1

        except Exception as e:
            print(f"⚠️ Error: {e}")
            continue

except KeyboardInterrupt:
    print("🛑 Stopping ambient vibe...")

finally:
    player.close()
    pygame.midi.quit()
    ser.close()
    print("✅ Cleaned up and exited.")
