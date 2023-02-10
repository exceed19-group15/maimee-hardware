#include "Beatmap.h"

Beatmap::Beatmap() {
  this->BeatmapID = -1;
  this->BeatCount = -1;
  this->BPM = -1;
  this->Duration = -1;
  this->beats = nullptr;
}

Beatmap::Beatmap(int beatmapID, int beatCount, int bpm, int duration, Beat* beats) {
  this->BeatmapID = beatmapID;
  this->BeatCount = beatCount;
  this->BPM = bpm;
  this->Duration = duration;
  this->beats = beats;
}

int Beatmap::getBeatmapID() {
  return this->BeatmapID;
}

int Beatmap::getBeatCount() {
  return this->BeatCount;
}

int Beatmap::getBPM() {
  return this->BPM;
}

int Beatmap::getDuration() {
  return this->Duration;
}

Beat* Beatmap::getBeats() {
  return this->beats;
}

bool Beatmap::isInitialized() {
  return this->BeatmapID != -1 && this->BeatCount != -1 && this->BPM != -1 && this->Duration != -1 && this->beats != nullptr;
}