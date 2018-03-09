/*    _   _ _ _____ _    _              _____     _ _     ___ ___  _  __
     /_\ | | |_   _| |_ (_)_ _  __ _ __|_   _|_ _| | |__ / __|   \| |/ /
    / _ \| | | | | | ' \| | ' \/ _` (_-< | |/ _` | | / / \__ \ |) | ' <
   /_/ \_\_|_| |_| |_||_|_|_||_\__, /__/ |_|\__,_|_|_\_\ |___/___/|_|\_\
                               |___/

   Copyright 2018 AllThingsTalk

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <ATT_IOT.h>
#include "credentials.h"

// 60 LEDS connected to pin 10 (SDD3)
#define WS_PIN 10
#define NUM_LEDS 60
#define ATTALK "api.allthingstalk.io"

// function called when command is received via MQTT
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient pubSub(ATTALK, 1883, callback, espClient);
ATTDevice device(ATT_ID, ATT_TOKEN);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, WS_PIN, NEO_GRB + NEO_KHZ800);

// called when NodeMCU starts up
void setup() {
  // blank the strip
  strip.begin();
  delay(10);
  strip.show();

  // setup serial
  Serial.begin(115200);
  delay(10);

  // setup the example
  setupWiFi();
  setupDevice();
}

// called in a loop. This is your main loop!
void loop() {
  // signal device to process any incomming messages
  device.process();

  // TODO: you can implement any logic that requires looping here
  delay(16);
}

void setupWiFi() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  // Connect to WiFi in STA (we're only the client) mode.
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupDevice() {
  // Set up AllThingsTalk device
  Serial.println();
  Serial.print("Connecting to ATTalk");

  while (!device.connect(&espClient, ATTALK)) Serial.println(".");
  Serial.println("");
  Serial.println("ATTalk connected");

  Serial.println();
  Serial.print("Connecting to MQTT");

  while (!device.subscribe(pubSub, ATT_TOKEN)) Serial.println(".");
  Serial.println("");
  Serial.println("MQTT connected");
}


void callback(char* t, byte* payload, unsigned int length) {
  // read topic
  String topic = String(t);

  // read payload
  char data[256];
  strncpy(data, (const char *)payload, length);
  data[length] = '\0';

  // get asset name from the topic
  int nameStart = topic.indexOf("asset/");
  int nameEnd = topic.lastIndexOf("/command");
  String assetName = topic.substring(nameStart + 6, nameEnd);

  Serial.print("Asset: ");
  Serial.println(assetName);

  Serial.println("Received data");
  Serial.println(data);

  StaticJsonBuffer<256> jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(data, length);
  JsonObject &value = root["value"];

  handleCommand(assetName, value);
}

void handleCommand(String assetName, JsonObject &payload) {
  if (assetName == "color") {
    uint8_t r = payload["r"];
    uint8_t g = payload["g"];
    uint8_t b = payload["b"];

    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
  }
}

