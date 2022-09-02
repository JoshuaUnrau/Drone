//
// Created by jpunr on 2022-07-01.
//
/*#include "Motors.h"
#include "ControlLoop.h"
//#include <Servo.h>
#include <Arduino.h>

Servo ESC [4];

double motorValue1 = 0;  // value from the analog pin
double motorValue2 = 0;
double motorValue3 = 0;  // value from the analog pin
double motorValue4 = 0;

double motorOffset1 = 0;  // value from the analog pin
double motorOffset2 = 0;
double motorOffset3 = 0;  // value from the analog pin
double motorOffset4 = 0;

int min_pulse_length = 1000;
int max_pulse_length = 2000;

int motors = 4;

bool PWMmode = false;

enum Axis{
  pitch,
  roll,
  yaw,
  alt
};

enum Motor{
  front_left,
  front_right,
  rear_left,
  rear_right
};

void setupMotorsPWM() {
    // Attach the ESC on pin 9
        //Short beeps means powered -> Long beep means armed
    Serial.println("Connecting to motors");
    delay(1000);
    ESC[0].attach(6,min_pulse_length,max_pulse_length); // (pin, min pulse width, max pulse width in microseconds)
    ESC[1].attach(9,min_pulse_length,max_pulse_length);
    ESC[2].attach(10,min_pulse_length,max_pulse_length); // (pin, min pulse width, max pulse width in microseconds)
    ESC[3].attach(11,min_pulse_length,max_pulse_length);
    for(int i = 0; i < motors; i++){
      ESC[i].writeMicroseconds(min_pulse_length);
    }
    delay(500);
    for(int i = 0; i < motors; i++){
      ESC[i].writeMicroseconds(max_pulse_length);
    }
}

void setupMotors(){
  setupMotorsPWM();
}

void setMotorPower(Motor motor, int power) {
  switch(motor){
    case front_left:
      ESC[motor].writeMicroseconds(power);
    case front_right:
      ESC[motor].writeMicroseconds(power);
    case rear_left:
      ESC[motor].writeMicroseconds(power);
    case rear_right:
      ESC[motor].writeMicroseconds(power);
    default:
      break;
  }
}

//Beep on throttle up means min throttle is two high https://discuss.ardupilot.org/t/constantly-beeping-esc-and-jerky-motor/37679#:~:text=Your%20min%20throttle%20is%20to,motor%20sometimes%20moves%20a%20bit.
void setTotalMotorPower(double power) {
  int motorValue = map(power, 0, 100, 1000, 2000);
  for(int i = 0; i < motors; i++){
    setMotorPower(i, power);
  }
}

double pitchPID [3] = {1, 1, 1};
double rollPID [3] = {1, 1, 1};
double yawPID [3] = {1, 1, 1};
double altPID [3] = {1, 1, 1};

double pitchError [3] = {0, 0, 0};
double rollError [3] = {0, 0, 0};
double yawError [3] = {0, 0, 0};
double altError [3] = {0, 0, 0};


double getPID(Axis axis){
  switch(axis){
    case pitch:
      return pitchError[0]*pitchPID[0] + pitchError[1]*pitchPID[1] + pitchError[2]*pitchPID[2];
    case roll:
      return rollError[0]*rollPID[0] + rollError[1]*rollPID[1] + rollError[2]*rollPID[2];
    case yaw:
      return yawError[0]*yawPID[0] + yawError[1]*yawPID[1] + yawError[2]*yawPID[2];
    case alt:
      return altError[0]*altPID[0] + altError[1]*altPID[1] + altError[2]*altPID[2];
    default:
      break;
  }
  return 0;
}

//Motor positions
//    ^^ 
//  1 |=| 2
//  3 |=| 4

void setMotorPower(){
  motorValue1 = (getPID(pitch)+getPID(roll)+getPID(yaw)+getPID(alt))*motorOffset1;
  motorValue2 = (getPID(pitch)-getPID(roll)-getPID(yaw)+getPID(alt))*motorOffset2;
  motorValue3 = (-getPID(pitch)+getPID(roll)-getPID(yaw)+getPID(alt))*motorOffset3;
  motorValue4 = (-getPID(pitch)-getPID(roll)+getPID(yaw)+getPID(alt))*motorOffset4;
  setMotorPower(front_left, motorValue1);    // Send the signal to the ESC
  setMotorPower(front_right, motorValue2);
  setMotorPower(rear_left, motorValue3);
  setMotorPower(rear_right, motorValue4);
}

void motorLoop() {
    // if(motorPower > 20) //To test that stuttering is not a software issue
    //   motorPower = 20;
    setMotorPower();
    setTotalMotorPower(motorPower);
}
*/