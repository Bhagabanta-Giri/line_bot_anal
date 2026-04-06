#ifndef HARDWARE_H
#define HARDWARE_H
#include <Arduino.h>
#include "include.h"

extern void setMotors(int leftSpeed, int rightSpeed);
extern void stopMotors();
extern void forward(int spd);
extern void turnRight(int spd);

#endif