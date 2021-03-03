#include <Arduino.h>
#include "ACTUATOR.h"

#define PULSE_PER_INCH 500
#define Hall_MIN_VAL 50
#define Hall_MAX_VAL 4000
#define Hall_FeedBack_Pin 2
#define AIN1 3
#define AIN2 4
#define PWMA 5
#define STBY 6

const int offsetA = 1;
const int offsetB = 1;
long Current_Pos = 0;

Actuator myActuator = Actuator(AIN1, AIN2, PWMA, offsetA, STBY);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Wait for the actuator to return to zero position");
  myActuator.Attach_FeedBack(FEEDBACK_TYPE_HALL_SENSOR, Hall_FeedBack_Pin, Hall_MIN_VAL, Hall_MAX_VAL, PULSE_PER_INCH);
  myActuator.reset();
  Current_Pos = myActuator.read();
}

void loop() {
  // put your main code here, to run repeatedly:
  int Speed = 255;
  Serial.println("Enter Position In Inches");
  while(Serial.available()>0)
  {
    int Inches = Serial.read();
    myActuator.drive(Speed,Inches);
  }
}