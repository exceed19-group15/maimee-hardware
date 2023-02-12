#include "Beatmap.h"

Beatmap::Beatmap() {
  this->BeatmapID = -1;
  this->BeatCount = -1;
  this->NoteCount = -1;
  this->Duration = -1;
  this->beats = nullptr;
}

Beatmap::Beatmap(int beatmapID, String songName, int beatCount, int duration,int lightShowTime, Beat* beats) {
  this->BeatmapID = beatmapID;
  this->songName = songName;
  this->BeatCount = beatCount;
  this->NoteCount = 0;
  this->Duration = duration;
  this->lightShowTime = lightShowTime;

  this->beats = beats;

  for (int i = 0; i < beatCount; i++) {
    if (beats[i].getPadNum() != -1) {
      this->NoteCount++;
    }
  }
}

int Beatmap::getBeatmapID() {
  return this->BeatmapID;
}

String Beatmap::getSongName() {
  return this->songName;
}

int Beatmap::getBeatCount() {
  return this->BeatCount;
}

int Beatmap::getNoteCount() {
  return this->NoteCount;
}

int Beatmap::getDuration() {
  return this->Duration;
}

Beat* Beatmap::getBeats() {
  return this->beats;
}

void Beatmap::resetBeats() {
  for (int i = 0; i < this->BeatCount; i++) {
    this->beats[i].setHit(false);
  }
}