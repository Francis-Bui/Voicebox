// Author @Francis-Bui

#include <SPI.h>     // include Arduino SPI library
#include <SD.h> 
#include "TMRpcm.h" // library for asynchronous _playback of PCM/WAV

#define _play 2
#define _speakerPin 9

TMRpcm audio;
 
File root;
File entry = root.openNextFile();

int buttonState;
int lastButtonState = LOW;

long lastDebounceTime = 0;
long debounceTimer = 3000;

int modeCount = 0;

char wantedChar = 'a';
char currentChar;

bool isOn;


void setup(void) {
  Serial.begin(9600);
  pinMode(_play, INPUT_PULLUP);
 
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("Failed!");
    while(true);  // stay here.
  }

  Serial.println("OK!");

  audio.speakerPin = _speakerPin;

  root = SD.open("/");

  audio.setVolume(7);    //   0 to 7. Set volume level
  audio.quality(1);      //  Set 1 for 2x oversampling Set 0 for normal
}

void loop() {

    debouncer();

    if (isOn == true) {
        modeCount++; // add is_playing to add
        
        if (modeCount == 0) {
            wantedChar = 'a';
        }

        if (modeCount == 1) {
            wantedChar = 'b';
        }

        if (modeCount == 2) {
            modeCount = 0; // reset mode back to 0
            wantedChar = 'a';
        }

        isOn = false;
    }

    currentChar = String(entry.name()).charAt(0); // String for the first character of the file

    if ((!digitalRead(_play)) && (currentChar == wantedChar) && !audio.isPlaying()){
        playLine();
    } else {
        File entry = root.openNextFile();
          if (!entry) {
            root.rewindDirectory();
            return;
          }
    }

}

void playLine() {
    int randNumber = random(0, 5);
    int i = 0;

    for (randNumber > i; i++;){
        File entry = root.openNextFile(); //skips a random amount of files (between 1-5)
    }

    audio.play(entry.name()); // plays current file
}

void debouncer() {

    int reading = digitalRead(_play);

    if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset last debounce time upon state change
}

    if ((millis() - lastDebounceTime) > debounceTimer) {

        if (reading != buttonState) {
            buttonState = reading; // Update button state upon change

        if (buttonState == HIGH) {
            isOn = true;
    }
    }
}
  lastButtonState = reading; // Last known button state is the new reading
}
 
