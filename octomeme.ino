#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const bool debugMode = true;

const int ledPins[] = {
  13, 12, 11, 10, 9, 8, 7, 6
};

const int sensorPins[] = {
  A0, A1, A2, A3, A4, A5, A6, A7
};

const int padsNotes[] = {
  40, 42, 44, 46, 47, 48, 49, 50
};

const int threshold = 10;

int sensorReading = 0;
int ledState = LOW;


void setup() {
  for (int i = 0; i <= 7; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  MIDI.begin(MIDI_CHANNEL_OMNI);
  if (debugMode) {
    Serial.begin(9500);
  } else {
    Serial.begin(115200);
  }
}

void playNote(int note, int velocity) {
  velocity = map(velocity, 0, 1024, 0, 127);
  if (debugMode) {
    Serial.println("[NOTE "+String(note)+" ON] Vel: "+String(velocity));
  } else {
    MIDI.sendNoteOn(note, velocity ,1);  // Send a Note (pitch 42, velo 127 on channel 1)
  }
}

void stopNote(int note) {
  if (debugMode) {
    Serial.println("[NOTE "+String(note)+" OFF]");
  } else {
    MIDI.sendNoteOff(note,0,1);
  }
}

void playPad(int padNumber, int sensorReading) {
  if (debugMode) {
    Serial.println("[PAD "+String(padNumber)+" TRIGGERED]");
  }
  int ledIntensity = map(sensorReading, 0, 1024, 0, 1024);
  analogWrite(ledPins[padNumber], ledIntensity);
  playNote(padsNotes[padNumber], sensorReading);
  delay(1);
  analogWrite(ledPins[padNumber], 0);
  stopNote(padsNotes[padNumber]);
}

void readSensors() {
  for(int i=0; i<=7; i++) {
    sensorReading = analogRead(sensorPins[i]);
    if (sensorReading >= threshold) {
      playPad(i, sensorReading);
    }
  }
}

void loop() {
  readSensors();
  delay(1);
}

