int currentSound = 0;
int filesNb;
String files[64];

AudioFileSourceSD *file;
AudioOutputI2S *out;

void setupAudio() {
  filesNb = 0;
  getAudioFiles(SD.open("/"), files, &filesNb);

  SPIFFS.begin();
  file = new AudioFileSourceSD();
  out = new AudioOutputI2S(0, 0, 32, 0);
  mp3 = new AudioGeneratorMP3();
  playNextSound();
}

void playNextSound() {
  if (DEBUG) {
    Serial.print("Playing current sound ");
    Serial.println(files[currentSound]);
  }
  file->open(files[currentSound].c_str());
  mp3->begin(file, out);
  currentSound = (currentSound+1)%filesNb;
}

void getAudioFiles(fs::File root, String *files,int *filesNb) {
  if (!root) {
    if (DEBUG) Serial.println("Can't open directory");
    return;
  }

  if (!root.isDirectory()) {
    if (DEBUG) Serial.println("This is not a directory");
    return;
  }

  File file = root.openNextFile();
  int foundFiles = 0;
  while (file && foundFiles < 10) {
    if (!file.isDirectory()) {
      if (strstr(file.name(), "mp3")) {
        files[foundFiles] = file.name();
        foundFiles++;
      }
    }
    file = root.openNextFile();
  }
  *filesNb = foundFiles;
}
