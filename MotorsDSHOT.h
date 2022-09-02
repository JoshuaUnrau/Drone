//
// Created by jpunr on 2022-07-15.
//

#ifndef DRONECONTROLFULL_MOTORSDSHOT_H
#define DRONECONTROLFULL_MOTORSDSHOT_H


class MotorsDSHOT {

};
void setMotorPower(int i, int power);
void setTotalMotorPower(double power);
void setAllMotorPower();
void setupMotors();
void motorLoop();
void stopMotors();

extern double pitchPID [3];
extern double rollPID [3];
extern double yawPID [3];
extern double altPID [3];

extern double pitchError [3];
extern double rollError [3];
extern double yawError [3];
extern double altError [3];

extern double motorOffset1;  // value from the analog pin
extern double motorOffset2;
extern double motorOffset3;  // value from the analog pin
extern double motorOffset4;

#endif //DRONECONTROLFULL_MOTORSDSHOT_H
