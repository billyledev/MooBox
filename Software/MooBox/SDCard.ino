void setupSD() {
  while (!SD.begin(D0)) {
    if (DEBUG) Serial.println("Error talking to SD card!");
    delay(5000);
  }
}
