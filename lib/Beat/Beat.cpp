#include "Beat.h"

Beat::Beat() {
  this->padNum = -1;
  this->timestamp = -1;
  this->frequency = -1;
  this->hit = false;
}



Beat::Beat(int padNum, int timestamp, int frequency) {
  this->padNum = padNum;
  this->timestamp = timestamp;
  this->frequency = frequency;
  this->hit = false;
}

int Beat::getPadNum() {
  return this->padNum;
}

int Beat::getTimestamp() {
  return this->timestamp;
}

int Beat::getFrequency() {
  return this->frequency;
}

void Beat::setHit(bool hit) {
  this->hit = hit;
}

bool Beat::getHit() {
  return this->hit;
}

bool Beat::isInitialized() {
  return this->padNum != -1 && this->timestamp != -1 && this->frequency != -1;
}