/******************************************************************************

******************************************************************************/

#include "ACTUATOR.H"
#include <Arduino.h>

Actuator actuator(0,0,0,0,0); //Singleton Class
void ISR_Flag()
{
   actuator.ISR_Flag_Callback();
}

void Actuator::ISR_Flag_Callback()
{
    if(micros()-p_Last_Pulse_Time > p_Debounce_Time)
    {
        p_Num_Pulse++;
        p_Last_Pulse_Time += p_Debounce_Time;
    }
}

Actuator::Actuator(int In1pin, int In2pin, int PWMpin, int offset, int STBYpin)
{
  In1 = In1pin;
  In2 = In2pin;
  PWM = PWMpin;
  Standby = STBYpin;
  Offset = offset;

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(PWMpin, OUTPUT);
  pinMode(Standby, OUTPUT);
}

int Actuator::drive(int speed)
{
   digitalWrite(Standby, HIGH);
   speed = speed * Offset;
   if(p_FeedBack_Attached)
   {
      if(p_type == FEEDBACK_TYPE_HALL_SENSOR)
      {
         if(speed>0)
         {
            Actuator::extend(speed);
            if(millis()-p_Prev_Timer > 100)
            {
               p_Prev_Timer = millis();
               p_Pos += p_Num_Pulse;
               p_Num_Pulse = 0;
               if(p_Pos == p_Prev_Pos || p_Pos == p_max) p_Pos = p_max;
               else p_Prev_Pos = p_Pos;
               return p_Pos;
            }
         }
         else
         {
            Actuator::retract(speed);
            if(millis()-p_Prev_Timer > 100)
            {
               p_Prev_Timer = millis();
               p_Pos -= p_Num_Pulse;
               p_Num_Pulse = 0;
               if(p_Pos == p_Prev_Pos || p_Pos == p_min) p_Pos = p_max;
               else p_Prev_Pos = p_Pos;
               return p_Pos;
            }
         }
      }
   }
   else
   {
      if (speed>=0) extend(speed);
      else retract(speed);
      return -1;
   }
   return -1;
}
void Actuator::drive(int speed, int distance)
{
   int x = distance*p_ppi; // Inches to (no: of puses)
   if(x>p_Pos && x!=p_Pos)
      drive(speed);
   else if(x<p_Pos && x!=p_Pos)
      drive(-speed);
   else brake();
}

void Actuator::extend(int speed)
{
   digitalWrite(In1, HIGH);
   digitalWrite(In2, LOW);
   analogWrite(PWM, speed);
}

void Actuator::retract(int speed)
{
   digitalWrite(In1, LOW);
   digitalWrite(In2, HIGH);
   analogWrite(PWM, speed);
}

void Actuator::brake()
{
   digitalWrite(In1, HIGH);
   digitalWrite(In2, HIGH);
   analogWrite(PWM, 0);
}

void Actuator::standby()
{
   digitalWrite(Standby, LOW);
}
void Actuator::Attach_FeedBack(int Type, const int FeedBack_Pin, int Min, int Max, int PPI)
{
   p_FeedBack_Attached = true;
   p_max = Max; p_min = Min; p_ppi = PPI;
   if(Type == FEEDBACK_TYPE_HALL_SENSOR)
   {
      p_type = FEEDBACK_TYPE_HALL_SENSOR;
      pinMode(FeedBack_Pin,INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(FeedBack_Pin),ISR_Flag,RISING);
   }
}
void Actuator::reset(int speed)
{
   p_Prev_Timer = millis();
   while(!p_Zero_Pos_Flag)
   {
      Actuator::retract(speed);
      if(p_Prev_Num_Pulse == p_Num_Pulse && (millis()-p_Prev_Timer > 100))
      {
            p_Zero_Pos_Flag = true;
            p_Num_Pulse = 0;
            Actuator::brake();
      }
      else
      {
         p_Prev_Num_Pulse = p_Num_Pulse;
         p_Prev_Timer = millis();
      }
   }
}
int Actuator::read()
{
   if(p_FeedBack_Attached)
   {
      return p_Pos;
   }
   else{
      return -1;
   }
}