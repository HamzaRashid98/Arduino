/**********************************
         Code Description
**********************************/
/*
 * This code is designed to read analog voltage at pin A0 
 * and convert it into curent value by using a 1W 1ohm resistor 
 * in series with the motor. Using a threshold value, the 
 * direction of motor can be reversed.
 * 
 * Components required:
 * 1. Arduino UNO, NANO or Mega.
 * 2. 1W 1ohm resistor.
 * 3. 5V DC motor.
 * 4. Motor Driver L298N.
 * 5. 5V 500mA - 1A power supply.
 * 6. Jumper Wires.
 * 7. Breadboard.
 */


// Motor Driver Pins
#define enA 10
#define in1 9
#define in2 8

//Current Monitor Pin
#define CurrentSensePin A0

/**********************************
  These values cannot be changed
**********************************/
boolean IsMotorStopped = false;

/**********************************
    These values can be changed
**********************************/
int Speed_of_Motor = 127; //range 0 - 255
int ReverseTime = 3;  //Time in Seconds for reversing of motor
float OverCurrent = 0.250; //range 0.0 - 5.0

/*
   Set the OverCurrent value on which you want to reverse the Motor.
   For the motor Current which is 250mA Loaded.

  V = I x R = 0.250A x 1ohm
  V = 0.250V

  So, the 250mV or 0.250V is the voltage which act as Over Current protection.

  I set 0.25 value on which motor changes direction.
*/

/**********************************
              Setup
**********************************/
void setup()
{
  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(CurrentSensePin, INPUT);
}

/**********************************
          Forever Loop
**********************************/
void loop()
{
  float Current = 0;
  /*
     1. We are continously monitoring the current.
     2. If the motor is consuimg normal current then it keeps moving in forward direction.
     3. If it exceeds the over current then we move the motor in reverse direction.
  */

  //Here, we are checking the current of motor.
  Current = GetMotorCurrent();

  if (Current < OverCurrent)
  {
    MoveForward();
  }
  else if (Current > OverCurrent)
  {
    //Before changing the direction, we will stop the motor
    if (IsMotorStopped == false)
    {
      MotorStop();
      IsMotorStopped = true;
    }
    //Now the motor will move in reverse direction
    MoveReverse();

    //Here the motor will keep in reverse direction for this time in Seconds. Change it according to your need.
    delay(ReverseTime);
  }
}

/**********************************
      Read the Motor Current
**********************************/
int GetMotorCurrent()
{
  int current = 0;
  current = (analogRead(CurrentSensePin)) * (5 / 1023);
  return current;
}
/**********************************
     Motor Forward Direction
**********************************/
void MoveForward()
{
  analogWrite(enA, Speed_of_Motor);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}
/**********************************
      Motor Revesre Direction
**********************************/
void MoveReverse()
{
  analogWrite(enA, Speed_of_Motor);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
/**********************************
            Motor Stop
**********************************/
void MotorStop()
{
  //Turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  delay(1);
}
