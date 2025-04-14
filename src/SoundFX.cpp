#include "SoundFX.h"
#include "DisplayUI.h" // so we can check if Silent Mode is active

// Giorno's Theme Melody definition

const int em7[] = { 64, 67, 71, 74 };     // E G B D (Em7)
const int amaj7[] = { 69, 73, 76, 81 };   // A C# E G# (Amaj7)
const int dm7[] = { 62, 65, 69, 72 };     // D F A C (Dm7)
const int gmaj7[] = { 67, 71, 74, 78 };   // G B D F# (Gmaj7)

const int* chordBank[] = { em7, amaj7, dm7, gmaj7 };
const int chordSizes[] = { 4, 4, 4, 4 }; // same size for now
const int chordCount = sizeof(chordBank) / sizeof(chordBank[0]);

static unsigned long lastChordSwitch = 0;
static int currentChordIndex = 0;
static int nextChordDuration = 3000; // default 3s

void maybeSwitchChord() {
  if (millis() - lastChordSwitch > nextChordDuration) {
    currentChordIndex = random(0, chordCount);
    nextChordDuration = random(1000, 6000); // 1 to 9 sec
    lastChordSwitch = millis();
  }
}



const int midi1[81][3] = {
  {Fb5, 667, 0}, {D5, 778, 111}, {D5, 111, 0}, {E5, 111, 0}, {F5, 333, 0}, {E5, 333, 0},
  {D5, 222, 0}, {Cb5, 333, 0}, {D5, 333, 0}, {E5, 222, 0}, {Fb5, 667, 0}, {B5, 556, 111},
  {B4, 222, 0}, {Cb5, 222, 0}, {D5, 333, 0}, {E5, 333, 0}, {D5, 222, 0}, {Cb5, 333, 0},
  {A5, 333, 0}, {G5, 222, 0}, {Fb5, 667, 0}, {D5, 778, 111}, {D5, 111, 0}, {E5, 111, 0},
  {F5, 333, 0}, {E5, 333, 0}, {D5, 222, 0}, {Cb5, 333, 0}, {D5, 333, 0}, {E5, 222, 0},
  {Fb5, 667, 0}, {B5, 556, 111}, {B5, 222, 0}, {Cb6, 222, 0}, {D6, 333, 0}, {G5, 333, 0},
  {Fb5, 222, 0}, {F5, 333, 0}, {D6, 333, 0}, {E6, 222, 0}, {Fb6, 667, 0}, {D6, 778, 111},
  {D6, 111, 0}, {E6, 111, 0}, {F6, 333, 0}, {E6, 333, 0}, {D6, 222, 0}, {Cb6, 333, 0},
  {D6, 333, 0}, {E6, 222, 0}, {Fb6, 667, 0}, {B6, 667, 0}, {B5, 222, 0}, {Cb6, 222, 0},
  {D6, 333, 0}, {E6, 333, 0}, {D6, 222, 0}, {Cb6, 333, 0}, {A6, 333, 0}, {G6, 222, 0},
  {Fb6, 667, 0}, {D6, 778, 111}, {D6, 111, 0}, {E6, 111, 0}, {F6, 333, 0}, {E6, 333, 0},
  {D6, 222, 0}, {Cb6, 333, 0}, {D6, 333, 0}, {E6, 222, 0}, {Fb6, 667, 0}, {B6, 556, 111},
  {B6, 222, 0}, {Cb6, 222, 0}, {D6, 333, 0}, {G6, 333, 0}, {Fb6, 222, 0}, {F6, 333, 0},
  {D6, 333, 0}, {Ab6, 222, 0}, {B6, 222, 0},
};

namespace SoundFX {
  void playGiornosTheme() {
    for (int i = 0; i < 81; i++) {
      tone(BUZZER_PIN, midi1[i][0], midi1[i][1]);
      delay(midi1[i][1] + midi1[i][2]);
    }
    noTone(BUZZER_PIN);
  }
}

const int midiScale[] = {
  262, // C4
  294, // D4
  330, // E4
  349, // F4
  392, // G4
  440, // A4
  494, // B4
  523  // C5
};


const int scaleSize = sizeof(midiScale) / sizeof(midiScale[0]);


void SoundFX::playSignalTone(int rssi) {
  if (DisplayUI::menuState == DisplayUI::SILENT_MODE) return;

  static unsigned long lastBeep = 0;
  if (millis() - lastBeep < 50) return;

  maybeSwitchChord(); // check if it's time to switch chord

  // Pick a note from current chord
  const int* chord = chordBank[currentChordIndex];
  int noteIndex = random(0, chordSizes[currentChordIndex]);
  int midiNote = chord[noteIndex];
  int freq = round(440.0 * pow(2, (midiNote - 69) / 12.0)); // MIDI to Hz

  tone(BUZZER_PIN, freq, 30);
  lastBeep = millis();
}




