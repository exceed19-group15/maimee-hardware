#ifndef BEATMAP_H
#define BEATMAP_H
#include <Beat.h>
#include <Arduino.h>


class Beatmap {
  private:
    int BeatmapID;
    String songName;
    int NoteCount;
    int BeatCount;
    int Duration;
    Beat* beats;
  public:
    Beatmap();
    Beatmap(int beatmapID, String songName, int beatCount, int duration, Beat* beats);
    int getBeatmapID();
    String getSongName();
    int getBeatCount();
    int getNoteCount();
    int getDuration();
    void resetBeats();
    Beat* getBeats();
};

#endif