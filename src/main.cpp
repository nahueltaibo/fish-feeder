#include <Arduino.h>

const int onBoardLed = 4; // Onboard LED pin

void setup()
{
  pinMode(onBoardLed, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  digitalWrite(onBoardLed, HIGH); // Turn on the LED
  Serial.println("LED is ON");
  delay(1000);

  digitalWrite(onBoardLed, LOW); // Turn off the LED
  Serial.println("LED is OFF");
  delay(1000);
}
