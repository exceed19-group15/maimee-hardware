#ifndef BEATMAP_H
#define BEATMAP_H
#include <Beat.h>


class Beatmap {
  private:
    int BeatmapID;
    int BeatCount;
    int BPM;
    int Duration;
    Beat* beats;
  public:
    Beatmap();
    Beatmap(int beatmapID, int beatCount, int bpm, int duration, Beat* beats);
    int getBeatmapID();
    int getBeatCount();
    int getBPM();
    int getDuration();
    Beat* getBeats();
    bool isInitialized();
};

#endif