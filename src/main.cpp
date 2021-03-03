#include <Arduino.h>
#include "ACTUATOR.h"

#define PULSE_PER_INCH 500
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
  myActuator.Attach_FeedBack(FEEDBACK_TYPE_HALL_SENSOR,Hall_FeedBack_Pin,50,4400);
  myActuator.reset();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Enter Position In Inches");
  while(Serial.available()>0)
  {
    int Inches = Serial.read();
    int Pos = Inches*PULSE_PER_INCH;
    if(Pos>Current_Pos && Pos!=Current_Pos)
      Current_Pos = myActuator.drive(255);
    else if(Pos<Current_Pos && Pos!=Current_Pos)
      Current_Pos = myActuator.drive(-255);
    else myActuator.brake();
  }
}