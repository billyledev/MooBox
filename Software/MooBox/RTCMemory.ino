struct {
  uint32_t crc32;
  uint8_t currentSound;
} rtcData;

void restoreDataFromRTC() {
  if (ESP.rtcUserMemoryRead(0, (uint32_t *)&rtcData, sizeof(rtcData))) {
    if (DEBUG) Serial.println("Reading RTC memory");

    uint32_t crcOfData = calculateCRC32((uint8_t *)&rtcData.currentSound, sizeof(rtcData.currentSound));
    if (crcOfData != rtcData.crc32) {
      if (DEBUG) Serial.println("CRC32 in RTC memory doesn't match CRC32 of data. Data is probably invalid!");
      return;
    }

    if (DEBUG) Serial.println("Data restored from RTC memory");
    if (DEBUG) Serial.printf("Restored current sound is %d\n", rtcData.currentSound);
    currentSound = rtcData.currentSound;
  }
}

void writeDataToRTC() {
  rtcData.currentSound = currentSound;
  rtcData.crc32 = calculateCRC32((uint8_t *)&rtcData.currentSound, sizeof(rtcData.currentSound));
  ESP.rtcUserMemoryWrite(0, (uint32_t *)&rtcData, sizeof(rtcData));
}

uint32_t calculateCRC32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xffffffff;
  while (length--) {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if (c & i) { bit = !bit; }
      crc <<= 1;
      if (bit) { crc ^= 0x04c11db7; }
    }
  }
  return crc;
}
