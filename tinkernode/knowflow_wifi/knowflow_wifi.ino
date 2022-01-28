#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DFRobot_Iot.h"
#include <Wire.h>
#include <OneWire.h>
#include <DFRobot_MAX17043.h>
#include <DFRobot_ADS1115.h>

//Time Setting
#define UPDATE_INTERVAL 1 * 60 * 1000
long lastUpdate = -UPDATE_INTERVAL;

//Sensor configuration
#define DS18B20_PIN D0
#define PHSENSOR_PIN 1
#define TDSSENSOR_PIN 0
#define SAMPLING_TIME 20
#define OFFSET -1.15

float averageVoltage = 0, tdsValue = 0, phVoltage = 0, phValue = 0;
float tempSensor;
float phSensor;

float PHData[SAMPLING_TIME];
float tempData[SAMPLING_TIME];
float TDSData[SAMPLING_TIME];

OneWire ds(DS18B20_PIN);
DFRobot_ADS1115 ads(&Wire);

//WiFi Information
// PLEASE MODIFY THE WIFI SETTING
const char* WIFI_SSID = "P40";
const char* WIFI_PASSWORD = "93686214f120"; 

//Device certificate information
// PLEASE MODIFY YOUR OWN ALIYUN SETTING
String ProductKey = "a1y6YcC8O4z";
String ClientId = "12345";
String DeviceName = "FfuemlbeY6ngXhfKizCo";
String DeviceSecret = "e9fa580c5fdc8068cb0e8330efec9034";

// Topic information
const char* pubTopic = "/sys/a1y6YcC8O4z/FfuemlbeY6ngXhfKizCo/thing/event/property/post";

//Identifier information
String tempIdentifier = "Temp";
String tdsIdentifier = "Tds";
String phIdentifier = "PH";
String batteryIdentifier = "Precentage";

//IOT Cloud domain information
String ALIYUN_SERVER = "iot-as-mqtt.cn-shanghai.aliyuncs.com";
uint16_t PORT = 1883;

DFRobot_Iot Iot;
WiFiClient espClient;
PubSubClient client(espClient);

//Wifi and IOT cloud functions

void connectWiFi(){
  Serial.println("Init Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting");
    delay(500);
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char * topic, byte * payload, unsigned int len){
  Serial.print("Recevice [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < len; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectIOTCloud(){
  Serial.println("Init MQTT connection");
  while (!client.connected()){
    if(client.connect(Iot._clientId, Iot._username, Iot._password)){
      Serial.println("MQTT connected");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(500);
    }
   
  }
}

// Sensor functions

float getTemp() {
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad


  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}

void runSensorSampling(){
  // Before Sampling, read data 10 times
  for (int i=0;i<10;i++){
    getTemp();
    ads.readVoltage(PHSENSOR_PIN);
    ads.readVoltage(TDSSENSOR_PIN);
    delay(100); 
  }

  for (int i=0;i<SAMPLING_TIME;i++){
    tempData[i] = getTemp();
    PHData[i] = ads.readVoltage(PHSENSOR_PIN);
    TDSData[i] = ads.readVoltage(TDSSENSOR_PIN);
    delay(300);
  }
}

void processSensorData(){
  tempSensor = getMedianNum(tempData, SAMPLING_TIME);
  
  phVoltage = avergearray(PHData, SAMPLING_TIME) / 1000;
  phSensor = 3.5 * phVoltage + OFFSET;
  
  averageVoltage = getMedianNum(TDSData, SAMPLING_TIME) / 1000 ; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float compensationCoefficient = 1.0 + 0.02*(tempSensor - 25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  float compensationVolatge = averageVoltage / compensationCoefficient;  //temperature compensation
  tdsValue = (133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
}

//Maths functions

double avergearray(float* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) {   //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  }
  else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;        //arr<min
        min = arr[i];
      }
      else {
        if (arr[i] > max) {
          amount += max;    //arr>max
          max = arr[i];
        }
        else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
}

float getMedianNum(float bArray[], int iFilterLen)
{
  float bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j;
  float bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}

void setup() {
  Serial.begin(9600);

  // Init ADS1115
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);   // 0x48
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);       // single-shot mode
  ads.setRate(eRATE_128);          // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
  ads.init(); 
  
  connectWiFi();
  Iot.init(ALIYUN_SERVER,ProductKey,ClientId,DeviceName,DeviceSecret);
  client.setServer(Iot._mqttServer, PORT);
  client.setCallback(callback);
  connectIOTCloud();
}

void loop() {
  if (!client.connected()){
      connectIOTCloud();
  }

  if (WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }
  
  if (millis() - lastUpdate >= UPDATE_INTERVAL){
    runSensorSampling();
    processSensorData();
  
    // output result to Serial
    Serial.print("Temp Value:");
    Serial.print(tempSensor);
    Serial.println("℃");
    Serial.print("PH Value:");
    Serial.println(phSensor);
    Serial.print("TDS Value:");
    Serial.print(tdsValue);
    Serial.println("ppm");
    Serial.println("=========");
    
    client.loop();
    //client.publish(pubTopic,("{\"id\":"+ClientId+",\"params\":{\""+tempIdentifier+"\":"+random(9)+"},\"method\":\"thing.event.property.post\"}").c_str());
    client.publish(pubTopic, ("{\"id\":" + ClientId + ",\"params\":{\"" + tempIdentifier + "\":" + tempSensor + ",\"" + phIdentifier + "\":" + phSensor + ",\"" + tdsIdentifier + "\":" + int(tdsValue) + "},\"method\":\"thing.event.property.post\"}").c_str());
    lastUpdate = millis();
  }
  
  delay(10);
}
