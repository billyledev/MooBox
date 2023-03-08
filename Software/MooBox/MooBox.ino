#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include "AudioGeneratorMP3.h"
#include "AudioFileSourceSD.h"
#include "AudioOutputI2S.h"

#define DEBUG true

AudioGeneratorMP3 *mp3;

void setup() {
  WiFi.mode(WIFI_OFF);
  if (DEBUG) {
    Serial.begin(115200);
    Serial.println("");
  }

  setupSD();
  performUpdate();
  restoreDataFromRTC();
  setupAudio();
}

void loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
    }
  } else {
    mp3->stop();
    if (DEBUG) Serial.println("Going into deep sleep");
    writeDataToRTC();
    ESP.deepSleep(0);
  }
}
