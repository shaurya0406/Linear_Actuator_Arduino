/******************************************************************************

******************************************************************************/

#include <Arduino.h>
#ifndef ACTUATOR_H
    #define ACTUATOR_H
#endif

//used in some functions so you don't have to send a speed
#define DEFAULTSPEED 255
#define FEEDBACK_TYPE_HALL_SENSOR 0
#define DEFAULT_MIN_FEEDBACK_VAL 0
#define DEFAULT_MAX_FEEDBACK_VAL 1023
#define DEFAULT_PULSE_PER_INCH 500
class Actuator
{
  public:
    // Constructor. Mainly sets up pins.
    Actuator(int In1pin, int In2pin, int PWMpin, int offset, int STBYpin);

    void ISR_Flag_Callback();

    // Drive in direction given by sign, at speed given by magnitude of the
	//parameter and default = 255 i.e full speed
    int drive(int speed = DEFAULTSPEED);

	// drive() upto a distance in Inches
    void drive(int speed,int distance);

	//currently not implemented
    //void stop();           // Stop motors, but allow them to coast to a halt.
    //void coast();          // Stop motors, but allow them to coast to a halt.

	//Stops motor by setting both input pins high
    void brake();

	//set the chip to standby mode.  The drive function takes it out of standby
	void standby();
    
    void reset(int speed = DEFAULTSPEED);
    void Attach_FeedBack(int Type,const int FeedBack_Pin,int Min = DEFAULT_MIN_FEEDBACK_VAL, int Max = DEFAULT_MAX_FEEDBACK_VAL, int PPI = DEFAULT_PULSE_PER_INCH);
    int read();

  private:
    //variables for the 2 inputs, PWM input, Offset value, and the Standby pin
	int In1, In2, PWM, Offset,Standby;
    //other private variables
    bool p_FeedBack_Attached;
    int p_type, p_feedBack_pin, p_min, p_max, p_ppi;
    int p_Pos = 0;
    int p_Prev_Pos = 0;
    int p_Num_Pulse;
    int p_Prev_Num_Pulse = 0;
    bool p_Zero_Pos_Flag = false;
    uint32_t p_Prev_Timer = 0;
    uint32_t p_Last_Pulse_Time;
    uint32_t p_Debounce_Time; // 500 microseconds
	//private functions that extend/retract the linear actuator
	void extend(int speed);
	void retract(int speed);
};


