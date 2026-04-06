#ifndef INCLUDE_H
#define INCLUDE_H

#include <Arduino.h>

#define IR_FL   2
#define IR_ML   3
#define IR_MR   4
#define IR_FR   5
#define ENA     6
#define IN1     7
#define IN2     8
#define IN3     9
#define IN4    10
#define ENB    11
#define ON_WHITE(pin)   (digitalRead(pin) == LOW)
#define ON_BLACK(pin)   (digitalRead(pin) == HIGH)
#define SPEED_BASE   200
#define SPEED_FAST   225
#define SPEED_TURN   170
#define SPEED_PIVOT  180
#define SPEED_SLOW   150

struct SensorState {
  bool fl, ml, mr, fr;  
};

#endif