#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "PCMS-FLAB"
#define WIFI_PASSWORD "fahb1lab"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBe77x5DmWHhwBuLVTJ2XjdGxoetFicBog"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://waterflow-sensor-7ed3e-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#define THRESHOLD 40

RTC_DATA_ATTR int wakes = 0;
const byte touchPin = T3;
const byte touchPin1 = T4;
const byte touchPin2 = T5;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = true;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    count++;
   
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      void callback() {
 
  }
  void setup(){
    Serial.begin(115200);
    Serial.printf("喚醒次數 :%d\n", wakes);
    wakes ++;
    delay(1000);
    Serial.println("進入睡眠 ");
    delay(1000);
    touchAttachInterrupt(touchPin, callback, THRESHOLD);
    touchAttachInterrupt(touchPin1, callback, THRESHOLD);
    touchAttachInterrupt(touchPin2, callback, THRESHOLD);

    esp_sleep_enable_touchpad_wakeup();
    esp_deep_sleep_start();
  }
  }
    }
    }
 
}
