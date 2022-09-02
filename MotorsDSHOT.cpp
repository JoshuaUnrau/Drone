//
// Created by jpunr on 2022-07-01.
//
#include "Motors.h"
#include "ControlLoop.h"
#include <Arduino.h>
#include <DShot.h>

/*
  redefine DSHOT_PORT if you want to change the default PORT
  Defaults
  UNO: PORTD, available pins 0-7 (D0-D7)
  Leonardo: PORTB, available pins 4-7 (D8-D11)
  e.g.
  #define DSHOT_PORT PORTD
*/
#define DSHOT_PORT PORTD
#define M1 4
#define M2 5
#define M3 6
#define M4 7

DShot esc(DShot::Mode::DSHOT300);

double motorValue1 = 0;  // value from the analog pin0
double motorValue2 = 0;
double motorValue3 = 0;  // value from the analog pin
double motorValue4 = 0;

double motorOffset1 = 1;  // value from the analog pin
double motorOffset2 = 1;
double motorOffset3 = 1;  // value from the analog pin
double motorOffset4 = 1;

int powerMin = 200;
int powerMax = 1000;


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

enum Motor {
  front_right,
  rear_left,
  front_left,
  rear_right
};

void setupMotorsDSHOT() {
  // Attach the ESC on pin 9
      //Short beeps means powered -> Long beep means armed
  Serial.println("Connecting to motors");
  esc.attach(M1);  
  esc.setThrottle(M1, 0, 0);
  esc.attach(M2);  
  esc.setThrottle(M2, 0, 0);
  esc.attach(M3);  
  esc.setThrottle(M3, 0, 0);
  esc.attach(M4);
  esc.setThrottle(M4, 0, 0);
  delay(1000);
}

void setupMotors() {
  setupMotorsDSHOT();
}

void setMotorPower(int motor, int power) {
  power += powerMin;
  if(power > powerMax) {
    power = powerMax;
  }
  if(power < 0){
    power = 0;
  }
  switch(motor){
    case front_left:
      esc.setThrottle(M3, power, 0);
      break;
    case front_right:
      esc.setThrottle(M1, power, 0);
      break;
    case rear_left:
      esc.setThrottle(M2, power, 0);
      break;
    case rear_right:
      esc.setThrottle(M4, power, 0);
      break;
    default:
      break;
  }
}

void stopMotors() {
  esc.setThrottle(M1, 0, 0);
  esc.setThrottle(M2, 0, 0);
  esc.setThrottle(M3, 0, 0);
  esc.setThrottle(M4, 0, 0);
}

//Beep on throttle up means min throttle is two high https://discuss.ardupilot.org/t/constantly-beeping-esc-and-jerky-motor/37679#:~:text=Your%20min%20throttle%20is%20to,motor%20sometimes%20moves%20a%20bit.
void setTotalMotorPower(double power) {
  int motorValue = map(power, 0, 100, 1000, 2000);
  for(int i = 0; i < motors; i++) {
    setMotorPower(i, power);
  }
  // setMotorPower(front_right, power); //no connection
  // setMotorPower(front_left, 0); //X wired backward
  // setMotorPower(rear_right, 0); //no connection
  // setMotorPower(rear_left, 0); //X
}

double pitchPID [3] = {1, 1, 1};
//P = 3
//8, 0, 10, filter = 0.5 unstable
//10, 0, 40 //not enough damping
double rollPID [3] = {5, 1, 20};
double yawPID [3] = {1, 1, 1};
double altPID [3] = {1, 1, 1};

double pitchError [3] = {0, 0, 0};
double rollError [3] = {0, 0, 0};
double yawError [3] = {0, 0, 0};
double altError [3] = {0, 0, 0};


double getPID(Axis axis) {
  switch(axis) {
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

void setAllMotorPower(){
  //Serial.print(" Error: ");
  Serial.print(rollError[0]*rollPID[0]);
  Serial.print("\t");
  Serial.print(rollError[1]*rollPID[1]);
  Serial.print("\t");
  Serial.print(rollError[2]*rollPID[2]);
  Serial.print("\t");
  Serial.print(motorValue1);
  Serial.print("\t");
  //Serial.print(" Roll PID: ");
  Serial.print(getPID(roll));
  Serial.println("\t");
  // motorValue1 = (getPID(pitch)+getPID(roll)+getPID(yaw)+getPID(alt))*motorOffset1;
  // motorValue2 = (getPID(pitch)-getPID(roll)-getPID(yaw)+getPID(alt))*motorOffset2;
  // motorValue3 = (-getPID(pitch)+getPID(roll)-getPID(yaw)+getPID(alt))*motorOffset3;
  // motorValue4 = (-getPID(pitch)-getPID(roll)+getPID(yaw)+getPID(alt))*motorOffset4;
  motorValue1 = -getPID(roll);
  motorValue2 = getPID(roll);
  motorValue3 = -getPID(roll);
  motorValue4 = getPID(roll);
  setMotorPower(front_left, motorValue1);    // Send the signal to the ESC
  setMotorPower(front_right, motorValue2);
  setMotorPower(rear_left, motorValue3);
  setMotorPower(rear_right, motorValue4);
}

void motorLoop() {
    // if(motorPower > 20) //To test that stuttering is not a software issue
    //   motorPower = 20;
    //setMotorPower();
    //Serial.println(motorPower);
    setAllMotorPower();
}