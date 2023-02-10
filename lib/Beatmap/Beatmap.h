#ifndef BEATMAP_H
#define BEATMAP_H
#include <Beat.h>


class Beatmap {
  private:
    int BeatmapID;
    int NoteCount;
    int BeatCount;
    int Duration;
    Beat* beats;
  public:
    Beatmap();
    Beatmap(int beatmapID, int beatCount, int duration, Beat* beats);
    int getBeatmapID();
    int getBeatCount();
    int getNoteCount();
    int getDuration();
    void resetBeats();
    Beat* getBeats();
};

#endif