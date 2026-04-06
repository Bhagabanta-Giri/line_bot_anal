#include <Arduino.h>
#include "include.h"

SensorState readWhite() {
  SensorState s;
  s.fl = ON_WHITE(IR_FL);
  s.ml = ON_WHITE(IR_ML);
  s.mr = ON_WHITE(IR_MR);
  s.fr = ON_WHITE(IR_FR);
  return s;
}

SensorState readBlack() {
  SensorState s;
  s.fl = ON_BLACK(IR_FL);
  s.ml = ON_BLACK(IR_ML);
  s.mr = ON_BLACK(IR_MR);
  s.fr = ON_BLACK(IR_FR);
  return s;
}