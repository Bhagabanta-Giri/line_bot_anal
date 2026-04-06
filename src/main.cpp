#include <Arduino.h>
#include "include.h"
#include "hardware.h"

enum Section {
  WAIT_START,
  A1_CURVED_WHITE,
  A2_ZIGZAG_WHITE,
  B1_BLACK,
  B2_PHI_WHITE,
  B3_BLACK,
  B4_SQUARE_BLACK,
  C1_RIGHT_EDGE,
  C2_LEFT_EDGE,
  C3_RIGHT_EDGE,
  C4_STRAW_HAT,
  D1_90_BLACK_EDGE,
  D2_TRIANGLE,
  D3_TRIANGLE,
  D4_TRIANGLE,
  D5_FINAL_GOAL,
  DONE
};

Section       currentSection = A1_CURVED_WHITE;
int           lastError      = 0;
unsigned long sectionTimer   = 0;

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

int computeError(SensorState s) {
  int pos = 0, cnt = 0;
  if (s.fl) { pos += -3; cnt++; }
  if (s.ml) { pos += -1; cnt++; }
  if (s.mr) { pos +=  1; cnt++; }
  if (s.fr) { pos +=  3; cnt++; }
  if (cnt == 0) return lastError * 3;  
  lastError = pos / cnt;
  return lastError;
}

void followLine(SensorState s, int baseSpeed) {
  int err        = computeError(s);
  int correction = (err * 40) + ((err - lastError) * 10);
  setMotors(constrain(baseSpeed + correction, 0, 255),
            constrain(baseSpeed - correction, 0, 255));
}

void followRightEdge(int spd) {
  if (ON_BLACK(IR_MR)) {
    setMotors(SPEED_TURN, spd);   
  } else {
    setMotors(spd, SPEED_TURN);   
  }
}

void followLeftEdge(int spd) {
  if (ON_BLACK(IR_ML)) {
    setMotors(spd, SPEED_TURN);  
  } else {
    setMotors(SPEED_TURN, spd);   
  }
}

bool isJunction(SensorState s) {
  int cnt = (s.fl ? 1 : 0) + (s.ml ? 1 : 0)
          + (s.mr ? 1 : 0) + (s.fr ? 1 : 0);
  return cnt >= 3;
}

void nextSection() {
  currentSection = (Section)((int)currentSection + 1);
  lastError      = 0;
  sectionTimer   = millis();
  stopMotors();
  delay(100);
}

void setup() {
  pinMode(IR_FL, INPUT); pinMode(IR_ML, INPUT);
  pinMode(IR_MR, INPUT); pinMode(IR_FR, INPUT);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  stopMotors();
  currentSection = A1_CURVED_WHITE; 
  sectionTimer = millis();
}

void loop() {

  switch (currentSection) {

    case A1_CURVED_WHITE: {
      SensorState s = readWhite();
      followLine(s, SPEED_BASE);
      if (isJunction(s) && millis() - sectionTimer > 1000) nextSection();
      break;
    }

    case A2_ZIGZAG_WHITE: {
      SensorState s   = readWhite();
      int err         = computeError(s);
      int correction  = (err * 50) + ((err - lastError) * 20);
      setMotors(constrain(SPEED_BASE + correction, 0, 255),
                constrain(SPEED_BASE - correction, 0, 255));
      if (isJunction(s) && millis() - sectionTimer > 1000) nextSection();
      break;
    }

    case B1_BLACK: {
      SensorState s = readBlack();
      followLine(s, SPEED_FAST);
      if (isJunction(s) && millis() - sectionTimer > 800) nextSection();
      break;
    }

    case B2_PHI_WHITE: {
      SensorState s = readWhite();
      followLine(s, SPEED_SLOW);
      if (isJunction(s) && millis() - sectionTimer > 2000) nextSection();
      break;
    }

    case B3_BLACK: {
      SensorState s = readBlack();
      followLine(s, SPEED_FAST);
      if (isJunction(s) && millis() - sectionTimer > 800) nextSection();
      break;
    }

    case B4_SQUARE_BLACK: {
      static int  cornerCount  = 0;
      static bool wasJunction  = false;
      SensorState s    = readBlack();
      bool         junc = isJunction(s);

      if (junc && !wasJunction) {
        cornerCount++;
        forward(SPEED_BASE);
        delay(400);
        turnRight(SPEED_PIVOT);
        delay(500);              //90 deg ke lie fine tune hau he. Isse mt chhuna
      }
      wasJunction = junc;
      if (!junc) followLine(s, SPEED_SLOW);

      if (cornerCount >= 4 && millis() - sectionTimer > 15000) {
        cornerCount = 0;
        nextSection();
      }
      break;
    }

    case C1_RIGHT_EDGE: {
      followRightEdge(SPEED_BASE);
      SensorState s = readBlack();
      if (isJunction(s) && millis() - sectionTimer > 1000) nextSection();
      break;
    }

    case C2_LEFT_EDGE: {
      followLeftEdge(SPEED_BASE);
      SensorState s = readBlack();
      if (isJunction(s) && millis() - sectionTimer > 1000) nextSection();
      break;
    }

    case C3_RIGHT_EDGE: {
      followRightEdge(SPEED_BASE);
      SensorState s = readBlack();
      if (isJunction(s) && millis() - sectionTimer > 1000) nextSection();
      break;
    }

    case C4_STRAW_HAT: {
      SensorState s = readWhite();
      followLine(s, SPEED_SLOW);
      if (isJunction(s) && millis() - sectionTimer > 3000) nextSection();
      break;
    }

    case D1_90_BLACK_EDGE: {
      followRightEdge(SPEED_BASE);
      SensorState s = readBlack();
      if (isJunction(s) && millis() - sectionTimer > 1500) nextSection();
      break;
    }

    case D2_TRIANGLE:
    case D3_TRIANGLE:
    case D4_TRIANGLE: {
      static int  triCorners = 0;
      static bool triWasJunc = false;
      SensorState s    = readWhite();
      bool         junc = isJunction(s);

      if (junc && !triWasJunc && millis() - sectionTimer > 500) {
        triCorners++;
        forward(SPEED_BASE);
        delay(400);
        turnRight(SPEED_PIVOT);
        delay(700);           // 120 deg ke lie hua he. mt chhuna
      }
      triWasJunc = junc;
      if (!junc) followLine(s, SPEED_BASE);

      if (triCorners >= 3 && millis() - sectionTimer > 2000) {
        triCorners = 0;
        nextSection();
      }
      break;
    }

    case D5_FINAL_GOAL: {
      SensorState s = readWhite();
      followLine(s, SPEED_SLOW);
      if (s.fl && s.ml && s.mr && s.fr) {
        stopMotors();
        currentSection = DONE;
      }
      break;
    }

    case DONE:
      stopMotors();
      break;

    default:
      SensorState s = readBlack();
      followLine(s, SPEED_BASE);

      break;
  }
}