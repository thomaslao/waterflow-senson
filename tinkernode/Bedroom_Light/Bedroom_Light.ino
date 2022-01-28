/*!
 * @file Bedroom_Light.ino
 *
 * @brief Simulate esp32 as a bedroom light and use Aliyun as a cloud platform. 
 * @n Subscribe to the switch status theme to enable remote switch control bedroom lights
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [Wuxiao](xiao.wu@dfrobot.com)
 * @version  V1.0
 * @date  2019-02-10
 * @get from https://www.dfrobot.com
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DFRobot_Iot.h"

#define BEDROOD_LIGHT  D4

/*Set WIFI name and password*/
const char * WIFI_SSID     = "P40";
const char * WIFI_PASSWORD = "93686214f120";

/*Configure device certificate information*/
String ProductKey = "a1AJPa5VTXk";
String ClientId = "12345";/*Custom id*/
String DeviceName = "edCgnDSEszMvfXaCDNJN";
String DeviceSecret = "1d2d686a30a9a40588cb7ae15b1c7152";

/*Configure the domain name and port number*/
String ALIYUN_SERVER = "iot-as-mqtt.cn-shanghai.aliyuncs.com";
uint16_t PORT = 1883;

/*Product identifier that needs to be operated*/
String Identifier = "LightStatus";

/*TOPIC that need to be published and subscribed*/
const char * subTopic = "/sys/a1AJPa5VTXk/${deviceName}/thing/event/property/post";//****set
const char * pubTopic = "/sys/a1AJPa5VTXk/${deviceName}/thing/service/property/set";//******post

DFRobot_Iot myIot;
WiFiClient espClient;
PubSubClient client(espClient);

static void openLight(){
  digitalWrite(BEDROOD_LIGHT, HIGH);
}

static void closeLight(){
  digitalWrite(BEDROOD_LIGHT, LOW);
}

void connectWiFi(){
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP Adderss: ");
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
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject((const char *)payload);
  if(!root.success()){
    Serial.println("parseObject() failed");
    return;
  }
  const uint16_t LightStatus = root["params"][Identifier];
  if(LightStatus == 1){
    openLight();
  }else{
    closeLight();
  }
  String tempMseg = "{\"id\":"+ClientId+",\"params\":{\""+Identifier+"\":"+(String)LightStatus+"},\"method\":\"thing.event.property.post\"}";
  char sendMseg[tempMseg.length()];
  strcpy(sendMseg,tempMseg.c_str());
  client.publish(pubTopic,sendMseg);
}

void ConnectCloud(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    /*A device connected to the cloud platform based on an automatically calculated username and password*/
    if(client.connect(myIot._clientId,myIot._username,myIot._password)){
      Serial.println("connected");
      client.subscribe(subTopic);
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup(){
  Serial.begin(115200);
  pinMode(BEDROOD_LIGHT,OUTPUT);
  
  /*Connect to WIFI*/
  connectWiFi();
  
  /*Initialize the configuration of Aliyun*/
  myIot.init(ALIYUN_SERVER,ProductKey,ClientId,DeviceName,DeviceSecret);
  
  client.setServer(myIot._mqttServer,PORT);
  
  /*Set the callback function to execute the callback function when receiving the subscription information*/
  client.setCallback(callback);
  
  /*Connect to the cloud platform*/
  ConnectCloud();
  
  /*Turn off the lights first*/
  closeLight();
  
  /*Publish information about turning off the lights*/
  client.publish(pubTopic,("{\"id\":"+ClientId+",\"params\":{\""+Identifier+"\":0},\"method\":\"thing.event.property.post\"}").c_str());
}

void loop(){
  if(!client.connected()){
    ConnectCloud();
  }
  client.loop();
}
