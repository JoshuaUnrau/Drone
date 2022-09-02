//
// Created by jpunr on 2022-07-01.
//

#include "Radio.h"
#include "ControlLoop.h"
#include "MotorsDSHOT.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial HC12(12, 11); // HC-12 TX Pin (Blue), HC-12 RX Pin (Purple)
int messageLength = 14;
String readBuffer = "";
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish

void setupRadio() {
    HC12.begin(9600);               // Serial port to HC12
}

// double targetRoll = 0;
// double targetPitch = 0;
// double targetYaw = 0;

int commandLength = 3; 
void parseCommand(String command) {
  targetPitch = command.substring(1, 4).toInt()-100;
  targetRoll = command.substring(4, 7).toInt()-100;
  targetYaw = command.substring(7, 10).toInt()-100;
  //Data error in transmission?
  motorPower = (command.substring(10, 13).toInt()-100)*2; //Temp as transmitter is broke
}

void parsePID(String command) {
  pitchPID[0] = command[1] - '0';
  pitchPID[1] = command[2] - '0';
  pitchPID[2] = command[3] - '0';

  rollPID[0] = command[4] - '0';
  rollPID[1] = command[5] - '0';
  rollPID[2] = command[6] - '0';

  yawPID[0] = command[7] - '0';
  yawPID[1] = command[8] - '0';
  yawPID[2] = command[9] - '0';

  altPID[0] = command[10] - '0';
  altPID[1] = command[11] - '0';
  altPID[2] = command[12] - '0';
}

void parseOffset(String command) {
  motorOffset1 = command[1] - '0';
  motorOffset2 = command[2] - '0';
  motorOffset3 = command[3] - '0';
  motorOffset4 = command[4] - '0'; //Temp as transmitter is broke
}

void simpleDebugRadio(){

}

void radioLoop() {
  byte read;
  if (HC12.available()) {        // If HC-12 has data
    //Serial.println("Reading Data");
    readBuffer = HC12.readStringUntil('F');
    //Serial.println(readBuffer);
    if (readBuffer.charAt(readBuffer.length() - 1) == 'D'){
      //Serial.println(readBuffer);

      if(readBuffer.charAt(0) == 'S') {
        motorPower = constrain(readBuffer.substring(1, 4).toInt()-100, 0, 100);
      }

      if(readBuffer.charAt(0) == 'C') {
        parseCommand(readBuffer);
      }

      if(readBuffer.charAt(0) == 'P') {
        parsePID(readBuffer);
      }

      if(readBuffer.charAt(0) == 'O') {
        parseOffset(readBuffer);
      }
      //delay(100);
      delayStart = 0;
      delayRunning = false;
      return; //We got our control commands now let the motor loop run (HC12 seems to buffer commands)
    } else {
      if ((millis() - delayStart) >= 1000) {
        haltDrone();
      } else if (!delayRunning){
        delayStart = millis(); 
        delayRunning = true;
      }
    }
    if(readBuffer.length() >= messageLength){
      // Serial.println("Message?");
      // Serial.println(readBuffer.charAt(0));
      // Serial.println(readBuffer.charAt(readBuffer.length() - 1));
      readBuffer = "";
    }
  } else {
    if (delayRunning && ((millis() - delayStart) >= 1000)) {
      haltDrone();
    } else if (!delayRunning){
        delayStart = millis(); 
        delayRunning = true;
    }
    //Serial.println("No Data");
    //delay(100);
  }
  
  //Serial.println(readBuffer.length());

  //https://www.elecrow.com/download/HC-12.pdf
  // String message = "tp: "+String(targetPitch, 3) + 
  //                  " ty: " + String(targetYaw, 3) + 
  //                  " tr: " + String(targetRoll, 3) +
  //                  " mp: " + String(motorPower, 3) +
  //                  " b: " + String(blink, 3) + '\n';
  
  
  // TODO: Find a non mutex method for bi-directional comms
  // if(targetPitch != prev_targetPitch || targetRoll != prev_targetRoll || targetYaw != prev_targetYaw|| motorPower != prev_motorPower) {
  //   String message = "S"+String(targetPitch+100)+String(targetRoll+100)+String(targetYaw+100)+String(motorPower+100)+"D";
  //   HC12.print(message);  // Send that data to HC-12
  //   prev_targetPitch = targetPitch;
  //   prev_targetRoll = targetRoll;
  //   prev_targetYaw = targetYaw;
  //   prev_motorPower = motorPower;
  // }
}