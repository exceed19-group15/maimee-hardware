#include <Arduino.h>
#include <WiFi.h>

#include <Beat.h>
#include <Beatmap.h>
#include <LiquidCrystal_I2C.h>

#include <LCEEDEE.h>
#include <beatmaps.h>
#include <requests.h>

#define RED 18
#define RED_SWITCH 23
#define YELLOW 19
#define YELLOW_SWITCH 27
#define GREEN 32
#define GREEN_SWITCH 26
#define BLUE 33
#define BLUE_SWITCH 25
#define BUZZER 12


LiquidCrystal_I2C lcd(0x27, 16, 2);

String currentState = "MENU";
String nextState = "MENU";
int beatmapID = -1;
long startMillis = 0;
unsigned long lastGameStateFetch = 0;
int hitCount = 0;
int hitOffset = 600;

Beatmap *currentBeatmap = nullptr;
int currentBeat = 0;
bool updateLCD = false;

long lastToneChange = 0;
int lastTone = 0;





void LCD_update() {
    if (updateLCD)
    {

      if (currentState == "MENU")
      {
        LCD_showMenu(&lcd);
      }
      else if (currentState == "PLAYING" || currentState == "FINISHED")
      {
        if (currentBeatmap == nullptr)
        {
          LCD_showMenu(&lcd);
        } else {

        LCD_showScore(&lcd, currentBeatmap->getSongName(), hitCount, currentBeatmap->getNoteCount());
        }
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
      unsigned int upperBound = beat->getTimestamp() + hitOffset;
      unsigned int lowerBound = beat->getTimestamp() - hitOffset;

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
    if (!GET_game_state(nextState, beatmapID))
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
        currentBeatmap = BEATMAPS[beatmapID];
        currentBeatmap->resetBeats(); 
        hitOffset = (currentBeatmap->getLightShowTime()/2);
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

  // WiFi.begin("group15", "thisisapassword");
  WiFi.begin("GTS 20", "Supakrit");
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