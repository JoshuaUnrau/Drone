//
// Created by jpunr on 2022-07-01.
//

#ifndef DRONECONTROLFULL_MOTORS_H
#define DRONECONTROLFULL_MOTORS_H


class Motors {

};

void setTotalMotorPower(double power);
void setupMotors();
void motorLoop();

extern double pitchPID [3];
extern double rollPID [3];
extern double yawPID [3];
extern double altPID [3];

extern double motorOffset1;  // value from the analog pin
extern double motorOffset2;
extern double motorOffset3;  // value from the analog pin
extern double motorOffset4;
#endif //DRONECONTROLFULL_MOTORS_H
