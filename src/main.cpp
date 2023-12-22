#ifndef ESP32
#error This code is designed to run on ESP32 platform, not Arduino nor ESP8266! Please check your Tools->Board setting.
#endif

// Select different ESP32 timer number (0-3) to avoid conflict
#define USE_ESP32_TIMER_NO 2

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP32_ISR_Servo.h"

#if !defined(LED_BUILTIN)
#define LED_BUILTIN 4 // Pin D4 mapped to pin GPIO4/FLASH LED of ESP32, control on-board FLASH LED
#endif

//------------SERVO SETTINGS----------------
#define PIN_SERVO 12
#define MIN_MICROS 500
#define MAX_MICROS 2500
#define SERVO_WAIT_POSITION 0
#define SERVO_FEED_POSITION 180
int servoIndex = -1;

//------------FEEDING SETTINGS----------------
#define FEEDING_INTERVAL_IN_HOURS 12                              // Change this to the interval you want to feed your fish
unsigned long previousMillis = 0;                                 // will store last time code was executed
const long interval = FEEDING_INTERVAL_IN_HOURS * 60 * 60 * 1000; // interval in milliseconds
// const long interval = 20 * 1000; // interval in milliseconds

void configureSerial()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  delay(200);
}

void configureServo()
{
  // Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

  servoIndex = ESP32_ISR_Servos.setupServo(PIN_SERVO, MIN_MICROS, MAX_MICROS);

  if (servoIndex != -1)
    Serial.println(F("Setup Servo OK"));
  else
    Serial.println(F("Setup Servo failed"));
}

void transitionToPosition(int targetPosition)
{
  int delayBetweenSteps = 10;

  int position = ESP32_ISR_Servos.getPosition(servoIndex);
  Serial.print(F("Transitioning from "));
  Serial.print(position);
  Serial.print(F(" to "));
  Serial.println(targetPosition);

  if (position < targetPosition)
  {
    for (; position <= targetPosition; position++)
    {
      ESP32_ISR_Servos.setPosition(servoIndex, position);
      // waits delayBetweenSteps ms for the servo to reach the position
      delay(delayBetweenSteps);
    }
  }
  else
  {
    for (; position > targetPosition; position--)
    {
      ESP32_ISR_Servos.setPosition(servoIndex, position);
      // waits delayBetweenSteps ms for the servo to reach the position
      delay(delayBetweenSteps);
    }
  }
}

bool isItTimeToFeed()
{
  unsigned long currentMillis = millis(); // get the current time

  // Check if the desired interval has passed
  if (currentMillis - previousMillis >= interval || previousMillis == 0) // We feed at startup and on the configured interval
  {
    // Save the current time for the next iteration
    previousMillis = currentMillis;

    Serial.println("It is time to feed the fish!");
    return true;
  }

  return false;
}

void feedFish()
{
  Serial.println(F("Feeding fish..."));

  int position;

  ESP32_ISR_Servos.enable(servoIndex);

  transitionToPosition(SERVO_WAIT_POSITION);

  transitionToPosition(SERVO_FEED_POSITION);

  ESP32_ISR_Servos.disable(servoIndex);

  Serial.println(F("Done feeding fish."));
}

void setup()
{
  configureSerial();

  Serial.print(F("\nStarting Fish feeder"));

  configureServo();
}

void loop()
{
  if (isItTimeToFeed())
  {
    feedFish();
  }
}
