#include <Arduino.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

#include "viola.h"

#define SPEAK_0 (0)
#define SPEAK_1 (1)
#define SPEAK_2 (2)
#define SPEAK_3 (3)
#define SPEAK_4 (4)
#define SPEAK_5 (5)
#define SPEAK_6 (6)
#define SPEAK_7 (7)
#define SPEAK_8 (8)
#define SPEAK_9 (9)
#define SPEAK_POINT (10)
#define SPEAK_NEGATIVE (11)
#define SPEAK_KILOGRAMS (12)
#define SPEAK_POUNDS (13)
#define SPEAK_NO (14)
#define SPEAK_BAD (15)
#define SPEAK_OK (16)
#define SPEAK_GOOD (17)
#define SPEAK_BATTERY (18)
#define SPEAK_SIGNAL (19)
#define SPEAK_PERCENT (20)
#define SPEAK_CONNECTING (21)
#define SPEAK_H50 (22)
#define SPEAK_H100 (23)
#define SPEAK_H250 (24)
#define SPEAK_L100 (25)
#define SPEAK_L250 (26)
#define SPEAK_W500 (27)
#define SPEAK_W1000 (28)

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *playingFile;
AudioFileSourcePROGMEM *files[27];

AudioOutputI2S *out;

/*
int audio_chain[] = {
  SPEAK_CONNECTING, SPEAK_W500,
  SPEAK_L100, SPEAK_L250, SPEAK_H50, SPEAK_H100, SPEAK_H250, SPEAK_W500,
  SPEAK_3, SPEAK_4, SPEAK_POINT, SPEAK_7, SPEAK_KILOGRAMS, SPEAK_W1000,
  SPEAK_BATTERY, SPEAK_GOOD, SPEAK_W1000,
  SPEAK_SIGNAL, SPEAK_NEGATIVE, SPEAK_5, SPEAK_2, SPEAK_PERCENT, SPEAK_W1000,
  SPEAK_NO, SPEAK_BAD, SPEAK_W1000,
  SPEAK_POUNDS, SPEAK_W1000
};
*/

#define SPEECH_LENGTH (10)

int audio_chain[] = {
  SPEAK_L100, SPEAK_W500, SPEAK_L100, SPEAK_L100, SPEAK_W1000, SPEAK_H250, SPEAK_SIGNAL, SPEAK_GOOD, SPEAK_BATTERY, SPEAK_OK
};

int audio_chain_pointer = 0;

void play_speech(int speech_command) {
  if (speech_command == SPEAK_W500) {
    delay(500);
  } else if (speech_command == SPEAK_W1000) {
    delay(1000);
  } else {
    Serial.printf("Playing file %i.\n", speech_command);
    if (wav->isRunning()) {
      wav->stop();
    }
    playingFile = files[speech_command];
    playingFile->seek(0, SEEK_SET);
    wav->begin(playingFile, out);
  }
}

void setup()
{

  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Serial.begin(115200);
  Serial.println("Setting up files.");
  int counter = 0;
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_0, sizeof(SPEECH_0));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_1, sizeof(SPEECH_1));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_2, sizeof(SPEECH_2));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_3, sizeof(SPEECH_3));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_4, sizeof(SPEECH_4));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_5, sizeof(SPEECH_5));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_6, sizeof(SPEECH_6));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_7, sizeof(SPEECH_7));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_8, sizeof(SPEECH_8));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_9, sizeof(SPEECH_9));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_10_POINT, sizeof(SPEECH_10_POINT));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_11_NEGATIVE, sizeof(SPEECH_11_NEGATIVE));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_12_KILOGRAMS, sizeof(SPEECH_12_KILOGRAMS));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_13_POUNDS, sizeof(SPEECH_13_POUNDS));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_14_NO, sizeof(SPEECH_14_NO));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_15_BAD, sizeof(SPEECH_15_BAD));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_16_OK, sizeof(SPEECH_16_OK));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_17_GOOD, sizeof(SPEECH_17_GOOD));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_18_BATTERY, sizeof(SPEECH_18_BATTERY));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_19_SIGNAL, sizeof(SPEECH_19_SIGNAL));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_20_PERCENT, sizeof(SPEECH_20_PERCENT));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_21_CONNECTING, sizeof(SPEECH_21_CONNECTING));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_22_HIGH_BEEP_50MS, sizeof(SPEECH_22_HIGH_BEEP_50MS));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_23_HIGH_BEEP_100MS, sizeof(SPEECH_23_HIGH_BEEP_100MS));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_24_HIGH_BEEP_250MS, sizeof(SPEECH_24_HIGH_BEEP_250MS));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_26_LOW_BEEP_100MS, sizeof(SPEECH_26_LOW_BEEP_100MS));
  files[counter++] = new AudioFileSourcePROGMEM(SPEECH_27_LOW_BEEP_250MS, sizeof(SPEECH_27_LOW_BEEP_250MS));
  out = new AudioOutputI2S(0, 1);
  out->SetGain(0.2);
  wav = new AudioGeneratorWAV();
  audio_chain_pointer = 0;
}


void loop()
{
  Serial.println(".1");
  if (wav->isRunning()) {
    Serial.println(".2");
    if (!wav->loop()) {
      wav->stop();
    }
  } else {
    Serial.printf("WAV done\n");
    Serial.println(".3");
    if (audio_chain_pointer == SPEECH_LENGTH) {
      Serial.println(".4");
      esp_deep_sleep_start();
    }
    Serial.println(".5");
    play_speech(audio_chain[audio_chain_pointer++]);
  }
}