#include <Arduino.h>
#include <Ultrasonic.h>
#include "Wire.h"
#include "DHT20.h"
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WifiClient.h>

#define BLYNK_TEMPLATE_ID "TMPL27Z0_a2MX"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "w7pbqCD37alAZUbB5-pS2u6L2srwDhek"
// motor
// humidity/temp
// light
// ultrasound
// soundSensor
// define FanPin 13
#define DHTSCLPin 21
#define DHTSDAPin 22
#define SoundSensorPin 38
#define LEDPin 32
#define FanPin 25
// ultrasound
#define UltrasoundTrigPin 15
#define UltrasoundEchoPin 13
int counter = 0;
bool lightStatus = false;
unsigned long lightTime = millis();
unsigned long temperatureTime = millis();
unsigned long motionTime = millis();
Ultrasonic ultrasonic(UltrasoundTrigPin, UltrasoundEchoPin);
DHT20 DHT(&Wire);
// String humidity;
// String temperature;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "NETGEAR2251";    // Your WiFi credentials.
char pass[] = "strongkayak245"; // Set password to "" for open networks.

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(SoundSensorPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  pinMode(FanPin, OUTPUT);
#if defined(ESP8266) || defined(ESP32)
  DHT.begin(DHTSCLPin, DHTSDAPin);
#else
  DHT.begin();
#endif
}
BLYNK_WRITE(V2)
  {
    int pinValue = param.asInt();
    if (pinValue == 1)
    {
      digitalWrite(FanPin, HIGH);
    }
    else
    {
      digitalWrite(FanPin, LOW);
    }
}

void loop()
{
  // Sound Activated Light Code
  int SoundLevel = analogRead(SoundSensorPin);
  if (SoundLevel > 1500 && millis() - lightTime > 5000)
  {
    Serial.println("Sound Detected");
    lightStatus = !lightStatus;
    Blynk.virtualWrite(V3, lightStatus);
    digitalWrite(LEDPin, lightStatus);
    lightTime = millis();
  }
  // Humidity and Temperature Code
  Blynk.run();
  int status = DHT.read();
  if (millis() - temperatureTime > 10000)
  {
    Blynk.virtualWrite(V1, DHT.getHumidity());
    Blynk.virtualWrite(V0, DHT.getTemperature() * 1.8 + 32);
    temperatureTime = millis();
  }
  long motionDetected = ultrasonic.Ranging(CM);
  if (motionDetected < 3 && millis() - motionTime > 10000)
  {
    Serial.println("Motion Detected");
    motionTime = millis();
    Blynk.logEvent("motion_detected","Motion Detected");
    Blynk.virtualWrite(V4,1);
  }

}