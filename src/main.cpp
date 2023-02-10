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

Beat beats1[17] = {
    Beat(-1, 0, 261),
    Beat(0, 1000, 261),
    Beat(1, 2000, 392),
    Beat(2, 3000, 392),
    Beat(3, 4000, 440),
    Beat(0, 5000, 440),
    Beat(1, 6000, 392),
    Beat(2, 7000, 0),
    Beat(-1, 8000, 349),
    Beat(3, 9000, 349),
    Beat(0, 10000, 329),
    Beat(1, 11000, 329),
    Beat(2, 12000, 293),
    Beat(3, 13000, 293),
    Beat(0, 14000, 261),
    Beat(1, 15000, 0),
    Beat(-1, 16000, 0)};

Beatmap beatmap1 = Beatmap(1, 17, 16000, beats1);

const String BASE_URL = "https://ecourse.cpe.ku.ac.th/exceed15";

String currentState = "MENU";
int beatmapID = -1;
Beatmap currentBeatmap = Beatmap();

unsigned long startMillis = 0;
unsigned long lastGameStateFetch = 0;
int hitCount = 0;
int currentBeat = 0;

void POST_final_score(int beatmap_id, int score, int hit, int miss)
{
  HTTPClient http;
  String URL = BASE_URL + "/game-state/";
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"game_state\": \"FINISHED\", \"beatmap_id\": " + String(beatmap_id) + "}";
  int responseCode = http.POST(payload);
  if (responseCode != 200)
  {
    Serial.println("Failed to post final score");
  }

  URL = BASE_URL + "/recent/";
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  payload = "{\"beatmap_id\": " + String(beatmap_id) + ", \"score\": " + String(score) + ", \"hit\": " + String(hit) + ", \"miss\": " + String(miss) + "}";
  responseCode = http.POST(payload);
  if (responseCode != 200)
  {
    Serial.println("Failed to post final score");
  }
}

TaskHandle_t TaskGame = NULL;
TaskHandle_t TaskUpdate = NULL;

void fetchGameState()
{
  DynamicJsonDocument document(2048);
  HTTPClient http;
  const String URL = BASE_URL + "/game-state/";
  http.begin(URL);
  int responseCode = http.GET();
  if (responseCode != 200)
  {
    Serial.println("Failed to fetch game state");
    return;
  }

  String payload = http.getString();
  deserializeJson(document, payload);

  if (currentState == "MENU" && document["game_state"] == "PLAYING")
  {
    startMillis = millis();
    Serial.println(startMillis);
    beatmapID = document["beatmap_id"].as<int>();
  }
  currentState = document["game_state"].as<String>();
  Serial.println(currentState); 
}

void gameLoop(void *param)
{
  while (1)
  {
    vTaskDelay(1);


    if (currentState != "PLAYING")
    {
      continue;
    }

    unsigned long currentTimestamp = millis() - startMillis;

    Beatmap *beatmap = &beatmap1;

    bool redOn = false;
    bool yellowOn = false;
    bool greenOn = false;
    bool blueOn = false;
    bool hit = false;

    if (currentTimestamp > beatmap->getDuration() && currentState == "PLAYING")
    {
      int score = (hitCount / (float)beatmap->getNoteCount()) * 100;
      POST_final_score(beatmapID, score, hitCount, beatmap->getNoteCount() - hitCount);
      currentState = "FINISHED";
      beatmap->resetBeats();
      currentBeat = 0;

      Serial.println("Finished");
      Serial.println("Score: " + score);
      Serial.println("Hit: " + String(hitCount));
      noTone(BUZZER);
      digitalWrite(RED, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(BLUE, LOW);
      continue;
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

      if (currentTimestamp >= beat->getTimestamp())
      {
        int freq = beat->getFrequency();
        if (freq > 0)
        {
          tone(BUZZER, freq);
        }
        else
        {
          noTone(BUZZER);
        }
      }

      if (beat->getPadNum() != -1 && currentTimestamp >= lowerBound && currentTimestamp <= upperBound && !beat->getHit())
      {
        switch (beat->getPadNum())
        {
        case 0:
          if (digitalRead(RED_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            continue;
          }
          redOn = true;
          break;
        case 1:
          if (digitalRead(YELLOW_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            continue;
          }
          yellowOn = true;
          break;
        case 2:
          if (digitalRead(GREEN_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            continue;
          }
          greenOn = true;
          break;
        case 3:
          if (digitalRead(BLUE_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            continue;
          }
          blueOn = true;
          break;
        }
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
}

void updateGameState(void *param)
{
  while (1)
  {
    fetchGameState();
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

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

  noTone(BUZZER);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // WiFi.begin("group15", "thisisapassword");
  WiFi.begin("TAGCH", "025123301");
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());

  xTaskCreatePinnedToCore(
      gameLoop,
      "gameLoop",
      10000,
      NULL,
      1,
      &TaskGame,
      0);

  xTaskCreatePinnedToCore(
      updateGameState,
      "updateGameState",
      10000,
      NULL,
      1,
      &TaskUpdate,
      1);
}

void loop()
{
}
