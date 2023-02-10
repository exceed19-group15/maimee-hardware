#ifndef SCORECOUNTER_H
#define SCORECOUNTER_H

#include <Arduino.h>

class ScoreCounter {
    private:
        int hitCount;
    public:
        ScoreCounter();
        int getHitCount();
        void increaseHitCount();
        int getTotalScore(int beatCount);
};

#endif