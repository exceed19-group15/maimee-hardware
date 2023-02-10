#include "ScoreCounter.h"

ScoreCounter::ScoreCounter() {
  this->hitCount = 0;
}

int ScoreCounter::getHitCount() {
  return this->hitCount;
}

void ScoreCounter::increaseHitCount() {
  this->hitCount++;
}

int ScoreCounter::getTotalScore(int beatCount) {
  return ((float)this->hitCount / (float)beatCount) * 100.0;
}