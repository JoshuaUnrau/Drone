// //
// // Created by jpunr on 2022-07-01.
// //

#include "IMU.h"
#include "MotorsDSHOT.h"
#include <MPU9255.h>//include MPU9255 library

MPU9255 mpu;
float X_out, Y_out, Z_out;  // Outputs
float Gyro_x, Gyro_y, Gyro_z;  // Outputs
float gyroAngleX;
double roll,pitch,rollF,pitchF=0;
float elapsedTime, currentTime, previousTime;

#define g 9.81 // 1g ~ 9.81 m/s^2
#define magnetometer_cal 0.06 //magnetometer calibration

 bool setupIMU() {
   bool init = !mpu.init();
   if(init) {
      Serial.println("IMU initialization success");
   } else {
      Serial.println("IMU initialization failed");
   }
   return init;
 }

 void getOrientation(){

 }

void calculateError(){
  pitchError[2] = pitchF - pitchError[0];
  pitchError[1] += pitchF;
  pitchError[0] = pitchF;

  rollError[2] = rollF - rollError[0];
  rollError[1] += rollF;
  rollError[0] = rollF;

  // Serial.print("Roll: ");
  // Serial.print(rollF);
  // Serial.print(" Error: ");
  // Serial.print(rollError[0]);
  // Serial.print(" X: ");
  // Serial.print(mpu.ax);
  // Serial.print(" Y: ");
  // Serial.print(mpu.ay);
  // Serial.print(" Z: ");
  // Serial.println(mpu.az);

}

 void runIMU(){
  mpu.read_acc();

  X_out = mpu.ax;
  Y_out = mpu.ay;
  Z_out = mpu.az;
  Gyro_x = mpu.gx;
  Gyro_x = mpu.gy;
  Gyro_x = mpu.gz;

  //roll = atan(Y_out / sqrt(pow(X_out, 2) + pow(Z_out, 2))) * 180 / PI;
  //pitch = atan(-1 * X_out / sqrt(pow(Y_out, 2) + pow(Z_out, 2))) * 180 / PI;

  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000;
  
  int signOfX = (X_out > 0) - (X_out < 0);
  float accAngleX = acos(Z_out/sqrt(pow(Z_out, 2) + pow(X_out, 2))) * 180/PI * signOfX;
  gyroAngleX += Gyro_X * elapsedTime; // deg/s * s = deg


  gyroAngleX = 0.95 * gyroAngleX + 0.05 * accAngleX; //Add accelerometer readings to gyro to reduce drift

  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  double filter = 0.02;
  // Low-pass filter
  rollF = filter * rollF + (1-filter) * roll;
  pitchF = filter * pitchF + (1-filter) * pitch;
   //Roll = atan2(mpu.ay, mpu.az) * 180/3.14159;
   //Serial.print(" Rol: ");
   //Serial.println(Roll);
  calculateError();
 }
