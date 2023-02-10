#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Beat.h>
#include <Beatmap.h>

#define RED 21
#define RED_SWITCH 23
#define YELLOW 19
#define YELLOW_SWITCH 22
#define GREEN 32
#define GREEN_SWITCH 26
#define BLUE 33
#define BLUE_SWITCH 25
#define BUZZER 12
// #define HIT_BUZZER 13

#define HIT_OFFSET 600

Beat mockBeats[16] = {
    Beat(0, 1000, 261),
    Beat(1, 2000, 261),
    Beat(2, 3000, 392),
    Beat(3, 4000, 392),
    Beat(0, 5000, 440),
    Beat(1, 6000, 440),
    Beat(2, 7000, 392),
    Beat(-1, 8000, 392),

    Beat(3, 9000, 349),
    Beat(0, 10000, 349),
    Beat(1, 11000, 329),
    Beat(2, 12000, 329),
    Beat(3, 13000, 293),
    Beat(0, 14000, 293),
    Beat(1, 15000, 261),

    Beat(-1, 16000, 0)
};

Beatmap MockBeatmap = Beatmap(0, 16, 0, 16000, mockBeats);

const String BASE_URL = "http://group15.exceed19.online" ;

// void fetchGameState(Game &game){
//   DynamicJsonDocument document(2048);
//   HTTPClient http;
//   const String URL = BASE_URL + "/game-state/";
//   http.begin(URL);
//   int responseCode = http.GET();
//   if (responseCode != 200) {
//     Serial.println("Failed to fetch game state");
//     return;
//   }

//   String payload = http.getString();
//   deserializeJson(document, payload);

//   if (game.getCurrentState() == "MENU" && document["state"] == "PLAYING") {
//     startMillis = millis();
//     game.setBeatmapID(document["beatmap_id"].as<int>());
//   }
//   game.setCurrentState(document["state"].as<String>());
// }

// void fetchBeatmap(Game &game) {
//   DynamicJsonDocument document(2048);
//   JsonArray beatsJson = document.createNestedArray("beats");

//   HTTPClient http;
//   const String URL = BASE_URL + "/beatmap/" + game.getBeatmapID() + "/";
//   http.begin(URL);
//   int responseCode = http.GET();
//   if (responseCode != 200) {
//     Serial.println("Failed to fetch beatmap");
//     return;
//   }

//   String payload = http.getString();
//   deserializeJson(document, payload);

//   Beat *beats = new Beat[beatsJson.size()];
//   beatsJson = document["beats"].as<JsonArray>();
//   beats = new Beat[beatsJson.size()];


//   for (int i = 0; i < beatsJson.size(); i++) {
//     beats[i] = Beat(
//       beatsJson[i]["padNum"].as<int>(),
//       beatsJson[i]["timestamp"].as<int>(),
//       beatsJson[i]["duration"].as<int>()
//     );
//   }

//   Beatmap beatmap = Beatmap(
//     document["beatmapId"].as<int>(),
//     document["beatCount"].as<int>(),
//     document["bpm"].as<int>(),
//     document["duration"].as<int>(),
//     beats
//   );

//   game.setBeatmap(beatmap);
// }


unsigned long score = 0;
int hitCount = 0;

int currentBeat = 0;
unsigned long startMillis = 0;

void setup()
{
  pinMode(RED, OUTPUT);
  pinMode(RED_SWITCH, INPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(YELLOW_SWITCH, INPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(GREEN_SWITCH, INPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BLUE_SWITCH, INPUT);
  pinMode(BUZZER, OUTPUT);
  // pinMode(HIT_BUZZER, OUTPUT);

  noTone(BUZZER);
  // noTone(HIT_BUZZER);

  Serial.begin(9600);
}

void loop()
{
  Beatmap *beatmap = &MockBeatmap;

  bool redOn = false;
  bool yellowOn = false;
  bool greenOn = false;
  bool blueOn = false;
    bool hit = false;

  unsigned long currentMillis = millis();
  unsigned long currentTimestamp = currentMillis - startMillis;

  if (currentTimestamp > beatmap->getDuration())
  {
    return;
  }

  for (int i = currentBeat; i < beatmap->getBeatCount(); i++)
  {
    Beat *beat = beatmap->getBeats() + i;
    unsigned int upperBound = beat->getTimestamp() + HIT_OFFSET;
    unsigned int lowerBound = beat->getTimestamp() - HIT_OFFSET;

    if (currentTimestamp > upperBound)
    {
      currentBeat++;
      continue;
    }

    if (currentTimestamp >= beat->getTimestamp()) {
      int freq = beat->getFrequency();
      if (freq > 0) {
        tone(BUZZER, freq);
      } else {
        noTone(BUZZER);
      }
    }

    if (currentTimestamp >= lowerBound && currentTimestamp <= upperBound && !beat->getHit())
    {
      switch (beat->getPadNum())
      {
      case 0:
        if (digitalRead(RED_SWITCH) == HIGH)
        {
          beat->setHit(true);
          hit = true;
          continue;
        }
        redOn = true;
        break;
      case 1:
        if (digitalRead(YELLOW_SWITCH) == HIGH)
        {
          beat->setHit(true);
          hit = true;
          continue;
        }
        yellowOn = true;
        break;
      case 2:
        if (digitalRead(GREEN_SWITCH) == HIGH)
        {
          beat->setHit(true);
          hit = true;
          continue;
        }
        greenOn = true;
        break;
      case 3:
        if (digitalRead(BLUE_SWITCH) == HIGH)
        {
          beat->setHit(true);
          hit = true;
          continue;
        }
        blueOn = true;
        break;
      }
    }
    if (hit)
    {
      hitCount++;
      Serial.println(hitCount);
    }

  }

  if (redOn)
  {
    digitalWrite(RED, HIGH);
  }
  else
  {
    digitalWrite(RED, LOW);
  }

  if (yellowOn)
  {
    digitalWrite(YELLOW, HIGH);
  }
  else
  {
    digitalWrite(YELLOW, LOW);
  }

  if (greenOn)
  {
    digitalWrite(GREEN, HIGH);
  }
  else
  {
    digitalWrite(GREEN, LOW);
  }

  if (blueOn)
  {
    digitalWrite(BLUE, HIGH);
  }
  else
  {
    digitalWrite(BLUE, LOW);
  }


}