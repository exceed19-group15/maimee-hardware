#ifndef BEAT_H
#define BEAT_H

class Beat {
    private:
        int padNum;
        int timestamp;
        int frequency;
        bool hit;
        int duration;
    public:
        Beat();
        Beat(int padNum, int timestamp, int frequency, int duration);
        int getPadNum();
        int getTimestamp();
        int getFrequency();
        bool getHit();
        void setHit(bool hit);
        bool isInitialized();
        int getDuration();
};

#endif