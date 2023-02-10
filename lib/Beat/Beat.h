#ifndef BEAT_H
#define BEAT_H

class Beat {
    private:
        int padNum;
        int timestamp;
        int frequency;
        bool hit;
    public:
        Beat();
        Beat(int padNum, int timestamp, int frequency);
        int getPadNum();
        int getTimestamp();
        int getFrequency();
        bool getHit();
        void setHit(bool hit);
        bool isInitialized();
};

#endif