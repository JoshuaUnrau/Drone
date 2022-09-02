//
// Created by jpunr on 2022-07-01.
//

#ifndef DRONECONTROLFULL_CONTROLLOOP_H
#define DRONECONTROLFULL_CONTROLLOOP_H


class ControlLoop {

};

extern int targetRoll;
extern int targetPitch;
extern int targetYaw;
extern int blink;
extern int motorPower;

extern int prev_targetRoll;
extern int prev_targetPitch;
extern int prev_targetYaw;
extern int prev_motorPower;

void haltDrone();

#endif //DRONECONTROLFULL_CONTROLLOOP_H
