#include <Arduino.h>    // Using the Arduino framework (not sure if we really wanna)


// GPIO pins used:
#define RLED 16
#define GLED 17
#define BLED 4

// PWM channels (dynamically allocated on ESP32)
#define RCHANNEL 0
#define BCHANNEL 1
#define GCHANNEL 2

#define IRSENSOR 34

// PWM setup
#define PWMFREQ 5000
#define PWMRES 13

bool IRSensorStatus;
bool prevIRSensorStatus;

// Emulate Arduino analogWrite():
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255)
{
  // calculate duty
  uint32_t duty = (PWMFREQ / valueMax) * min(value, valueMax);
  // write duty to LEDC
  ledcWrite(channel, duty);
}

void setup()
{

  pinMode(IRSENSOR, INPUT); // Not need, stated for clarity

  ledcSetup(RCHANNEL, PWMFREQ, PWMRES);
  ledcAttachPin(RLED, RCHANNEL);
  ledcSetup(BCHANNEL, PWMFREQ, PWMRES);
  ledcAttachPin(BLED, BCHANNEL);
  ledcSetup(GCHANNEL, PWMFREQ, PWMRES);
  ledcAttachPin(GLED, GCHANNEL);

  Serial.begin(9600);

  prevIRSensorStatus = false;
  IRSensorStatus = true;
}

void loop()
{
  delay(300);

  IRSensorStatus = digitalRead(IRSENSOR);

 /*
  Serial.print("IR: ");
  Serial.println(IRSensorStatus ? "HIGH" : "LOW");
 */

  if (IRSensorStatus != prevIRSensorStatus)
  {
    // Status change.
    if (IRSensorStatus)
    {
      // Sensor goes high.
      Serial.println("Sensor goes HIGH");
      for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5)
      {
        // Fade up RED, fade down GREEN
        ledcAnalogWrite(RCHANNEL, fadeValue);
        ledcAnalogWrite(GCHANNEL, 255 - fadeValue);
        Serial.print(".");
        delay(20);
      } // for
    }
    else
    {
      // Sensor goes low
      Serial.println("Sensor goes LOW");
      for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5)
      {
        // Fade up GREEN, down RED.
        ledcAnalogWrite(GCHANNEL, fadeValue);
        ledcAnalogWrite(RCHANNEL, 255 - fadeValue);
        delay(20);
      }
    }
  }
  prevIRSensorStatus = IRSensorStatus;
}
