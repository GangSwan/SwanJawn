import serial
import mido
import time

# Open the Swan Jawn serial port (adjust as needed)
ser = serial.Serial('COM3', 115200, timeout=1)  # Use correct port!

# Set up virtual MIDI output
outport = mido.open_output('SwanJawn MIDI Out', virtual=True)

print("🎵 Swan Jawn MIDI bridge is live!")

while True:
    try:
        line = ser.readline().decode().strip()
        if not line:
            continue

        # Expecting: millis,note,duration
        parts = line.split(',')
        if len(parts) != 3:
            continue

        millis, note, dur = map(int, parts)

        # Send note on
        outport.send(mido.Message('note_on', note=note, velocity=100))

        # Schedule note off later
        time.sleep(dur / 1000.0)
        outport.send(mido.Message('note_off', note=note, velocity=100))

        print(f"🎹 Note {note} played for {dur}ms")

    except KeyboardInterrupt:
        break
    except Exception as e:
        print(f"Error: {e}")
