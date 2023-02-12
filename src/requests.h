#include <HTTPClient.h>
#include <ArduinoJson.h>

const String BASE_URL = "https://ecourse.cpe.ku.ac.th/exceed15";
// const String BASE_URL = "http://group15.exceed19.online";

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


bool GET_game_state(String &nextState, int &beatmapID)
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