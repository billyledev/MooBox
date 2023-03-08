#define UPDATE_FILE_PATH "/system/update.bin"

void performUpdate() {
  if (DEBUG) Serial.println("Searching for update file...");
  File updateBin = SD.open(UPDATE_FILE_PATH);
  if (updateBin) {
    if (!updateBin.isDirectory()) {
      Serial.println("Update file found!");
      size_t updateSize = updateBin.size();

      if (updateSize > 0) {
        Serial.println("Starting update...");
        launchUpdate(updateBin, updateSize);
      }
    }

    updateBin.close();
    SD.remove(UPDATE_FILE_PATH);
    ESP.reset();
  } else {
    if (DEBUG) Serial.println("No update file found.");
  }
}

void launchUpdate(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {
    Update.writeStream(updateSource);
    Update.end();
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.println("Update finished!");
  } else {
    Update.printError(Serial);
    Serial.println("Error during update.");
  }
}
