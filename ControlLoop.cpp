//
// Created by jpunr on 2022-07-01.
//

#include <Arduino.h>

#include "ControlLoop.h"
#include "Radio.h"
//#include "Motors.h"
#include "MotorsDSHOT.h"
//#include "MPU6050.h"
#include "IMU.h"

// public double targetRoll = 0;
// extern double targetPitch = 0;
// extern double targetYaw = 0;

int targetRoll = 0;
int targetPitch = 0;
int targetYaw = 0;
int motorPower = 0;

int prev_targetRoll = 0;
int prev_targetPitch = 0;
int prev_targetYaw = 0;
int prev_motorPower = 0;

int blink = 1;

bool motorsOn = true;

enum Motor {
  front_right,
  rear_left,
  front_left,
  rear_right
};

void haltDrone() {
  stopMotors();
  Serial.println("Stopping motors");
  delay(2000);
  motorsOn = false;
}

double throttle = 0;
int target = 0;
int selectedMotor = 0;
void debug_serial() {
  Serial.println("Throttle: " + String(throttle));
  Serial.println("Target: " + String(target));
  Serial.println("Selected: " + String(selectedMotor));
  // if (HC12.available()) {
  //   HC12.parseInt();
  
  //1 works
  //
  if (Serial.available()) {
    String incomingByte = Serial.readString();
    if (incomingByte == "FL") {
      selectedMotor = front_left;
    } else if (incomingByte == "FR") {
      selectedMotor = front_right;
    } else if (incomingByte == "RL") {
      selectedMotor = rear_left;
    } else if (incomingByte == "RR") {
      selectedMotor = rear_right;
    } else if (incomingByte == "AM") {
      selectedMotor = 10;
    } else {
      target = incomingByte.toInt();
      if (target > 2047)
        target = 2047;
    }
  }
  if (target > throttle) {
    throttle++;
  } else if (target < throttle) {
    throttle--;
  }
  //setTotalMotorPower(throttle);
  if(selectedMotor == 10) {
    setTotalMotorPower(throttle);
  } else {
    setMotorPower(selectedMotor, throttle);
  }
  //setTotalMotorPower(throttle);
}

int main() {
  init();  //This needs to be called before you do anything
  Serial.begin(9600);
  setup();
  //pinMode(LED_BUILTIN, OUTPUT);
  while (true) {
    // if(blink > 0) {
    //     digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //     delay(1000);                       // wait for a second
    //     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    //     //delay(1000);                       // wait for a second
    // }
    runIMU();
    motorLoop();
    //radioLoop();
    //debug_serial();
    //Serial.println("Test");
    // if(motorsOn){
    //   motorLoop();
    // }
    //sensorLoop();
    //delay(1000);
  }
}

void setup() {
  //Serial.begin(9600);  // Serial port to computer
  Serial.println("Starting");
  //FIXME: Bug in IMU or radio setup causing serial to fail
  if (setupIMU()) {
    setupRadio();
    setupMotors();
  } else {
    //dont do that
    haltDrone();
  }
  setupMotors();
  Serial.println("Setup Complete");
  delay(2000);
}