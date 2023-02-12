#include "Beat.h"
Beat::Beat() {
  this->padNum = -1;
  this->timestamp = -1;
  this->frequency = -1;
  this->hit = false;
}



Beat::Beat(int padNum, int timestamp, int frequency, int duration) {
  this->padNum = padNum;
  this->timestamp = timestamp;
  this->frequency = frequency;
  this->duration = duration;

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


int Beat::getDuration() {
  return this->duration;
}
