#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#include <Beat.h>
#include <Beatmap.h>
#include <LiquidCrystal_I2C.h>
#include <LCEEDEE.h>

#define RED 18
#define RED_SWITCH 23
#define YELLOW 19
#define YELLOW_SWITCH 27
#define GREEN 32
#define GREEN_SWITCH 26
#define BLUE 33
#define BLUE_SWITCH 25
#define BUZZER 12

#define HIT_OFFSET 600

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
    Beat(-1, 16000, 0)
};

Beatmap beatmap1 = Beatmap(0, "Twinkle Twinkle", 17, 16000, beats1);

Beatmap *beatmaps = {&beatmap1};

// const String BASE_URL = "https://ecourse.cpe.ku.ac.th/exceed15";
const String BASE_URL = "http://group15.exceed19.online";

String currentState = "MENU";
String nextState = "MENU";
int beatmapID = -1;
long startMillis = 0;
unsigned long lastGameStateFetch = 0;
int hitCount = 0;

Beatmap *currentBeatmap = nullptr;
int currentBeat = 0;
bool updateLCD = false;

bool POST_final_score(int beatmap_id, int score, int hit, int miss)
{
  HTTPClient http;
  String URL = BASE_URL + "/recent";
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"beatmap_id\": " + String(beatmap_id) + ", \"score\": " + String(score) + ", \"hit\": " + String(hit) + ", \"miss\": " + String(miss) + "}";

  int responseCode = http.POST(payload);
  if (responseCode != 200)
  {
    Serial.println("Failed to post final score");
    return false;
  }

  URL = BASE_URL + "/game-state";
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  payload = "{\"game_state\": \"FINISHED\", \"beatmap_id\": " + String(beatmap_id) + "}";
  responseCode = http.POST(payload);
  if (responseCode != 200)
  {
    Serial.println("Failed to update to finished state");
    return false;
  }

  return true;
}


bool GET_game_state(String &nextState)
{
  DynamicJsonDocument document(2048);
  HTTPClient http;
  const String URL = BASE_URL + "/game-state";
  http.begin(URL);
  int responseCode = http.GET();
  if (responseCode != 200)
  {
    Serial.println("Failed to fetch game state");
    Serial.println("Response code: " + String(responseCode));
    return false;
  }

  String payload = http.getString();
  deserializeJson(document, payload);

  nextState = document["game_state"].as<String>();
  if (nextState == "PLAYING")
  {
    beatmapID = document["beatmap_id"].as<int>();
  }
  return true;
}



void LCD_update() {
    if (updateLCD)
    {

      if (currentState == "MENU")
      {
        LCD_showMenu(&lcd);
      }
      else if (currentState == "PLAYING" || currentState == "FINISHED")
      {
        LCD_showScore(&lcd, currentBeatmap->getSongName(), hitCount, currentBeatmap->getNoteCount());
      }
      else if (currentState == "GIVEUP")
      {
        LCD_showGiveUp(&lcd);
      }
    }
    updateLCD = false;
}

void gameLoop(void *param)
{
  while (1)
  {
    vTaskDelay(1);

    LCD_update();

    if (currentState != "PLAYING")
    {
      continue;
    }

    bool redOn = false;
    bool yellowOn = false;
    bool greenOn = false;
    bool blueOn = false;

    long currentTimestamp = millis() - startMillis;

    if (currentTimestamp > currentBeatmap->getDuration())
    {
      Serial.println(currentTimestamp);
      int score = (hitCount / (float)currentBeatmap->getNoteCount()) * 100;
      POST_final_score(beatmapID, score, hitCount, currentBeatmap->getNoteCount() - hitCount);
      nextState = "FINISHED";
      Serial.println("Finished");
      Serial.println("Hit: " + String(hitCount));
      continue;
    }

    for (int i = currentBeat; i < currentBeatmap->getBeatCount(); i++)
    {
      Beat *beat = currentBeatmap->getBeats() + i;
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
            updateLCD = true;
            continue;
          }
          redOn = true;
          break;
        case 1:
          if (digitalRead(YELLOW_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            updateLCD = true;
            continue;
          }
          yellowOn = true;
          break;
        case 2:
          if (digitalRead(GREEN_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            updateLCD = true;
            continue;
          }
          greenOn = true;
          break;
        case 3:
          if (digitalRead(BLUE_SWITCH) == HIGH)
          {
            beat->setHit(true);
            hitCount++;
            updateLCD = true;
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
    if (!GET_game_state(nextState))
    {
      continue;
    }
    if (currentState != nextState)
    {
      updateLCD = true;
      if (nextState == "PLAYING")
      {
        startMillis = millis();
        hitCount = 0;
        currentBeat = 0;
        currentBeatmap = &beatmaps[beatmapID];
        currentBeatmap->resetBeats(); 
      }
      else
      {
        noTone(BUZZER);
        digitalWrite(RED, LOW);
        digitalWrite(YELLOW, LOW);
        digitalWrite(GREEN, LOW);
        digitalWrite(BLUE, LOW);
      }
      currentState = nextState;
      Serial.println("State changed to " + currentState);
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

TaskHandle_t TaskGame = NULL;
TaskHandle_t TaskUpdate = NULL;

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

  LCD_setup(&lcd);

  noTone(BUZZER);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.begin("group15", "thisisapassword");
  // WiFi.begin("TAGCH", "025123301");
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