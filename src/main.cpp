#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

#define TIMER_INTERRUPT_DEBUG 1
#define ISR_SERVO_DEBUG 1

// Select different ESP32 timer number (0-3) to avoid conflict
#define USE_ESP32_TIMER_NO 2

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32_ISR_Servo.h"

#if !defined(LED_BUILTIN)
#define LED_BUILTIN 4 // Pin D4 mapped to pin GPIO4/FLASH LED of ESP32, control on-board FLASH LED
#endif

#define PIN_SERVO 12

// Published values for SG90 servos; adjust if needed
#define MIN_MICROS 800 // 544
#define MAX_MICROS 2450

int servoIndex1 = -1;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  delay(200);

  Serial.print(F("\nStarting ISR_MultiServos on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_ISR_SERVO_VERSION);

  // Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

  servoIndex1 = ESP32_ISR_Servos.setupServo(PIN_SERVO, MIN_MICROS, MAX_MICROS);

  if (servoIndex1 != -1)
    Serial.println(F("Setup Servo1 OK"));
  else
    Serial.println(F("Setup Servo1 failed"));
}

void loop()
{
  int position;

  if ((servoIndex1 != -1))
  {
    for (position = 0; position <= 180; position++)
    {
      // goes from 0 degrees to 180 degrees
      // in steps of 1 degree

      if (position % 30 == 0)
      {
        Serial.print(F("Servo1 pos = "));
        Serial.print(position);
      }

      ESP32_ISR_Servos.setPosition(servoIndex1, position);
      // waits 30ms for the servo to reach the position
      delay(50);
    }

    delay(1000);

    for (position = 180; position >= 0; position--)
    {
      // goes from 180 degrees to 0 degrees
      if (position % 30 == 0)
      {
        Serial.print(F("Servo1 pos = "));
        Serial.print(position);
      }

      ESP32_ISR_Servos.setPosition(servoIndex1, position);
      // waits 30ms for the servo to reach the position
      delay(30);
    }

    delay(5000);
  }
}