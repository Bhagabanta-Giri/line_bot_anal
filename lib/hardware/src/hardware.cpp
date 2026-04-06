#include "Arduino.h"
#include "include.h"

void setMotors(int leftSpeed, int rightSpeed) {
  int minPower = 125; 

  if (leftSpeed != 0) {
    bool reverse = (leftSpeed < 0);
    int power = abs(leftSpeed);
    if (power < minPower) power = minPower; 
    digitalWrite(IN1, !reverse ? HIGH : LOW);
    digitalWrite(IN2, !reverse ? LOW : HIGH);
    analogWrite(ENA, constrain(power, 0, 255));
  } else {
    analogWrite(ENA, 0);
  }
  if (rightSpeed != 0) {
    bool reverse = (rightSpeed < 0);
    int power = abs(rightSpeed);
    if (power < minPower) power = minPower;

    digitalWrite(IN3, !reverse ? HIGH : LOW);
    digitalWrite(IN4, !reverse ? LOW : HIGH);
    analogWrite(ENB, constrain(power, 0, 255));
  } else {
    analogWrite(ENB, 0);
  }
}

void stopMotors() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void forward(int spd)    { setMotors(spd, spd); }
// void turnLeft(int spd)   { setMotors(-spd, spd); }        //not used
void turnRight(int spd)  { setMotors(spd, -spd); } 
// void curveLeft(int spd)  { setMotors(SPEED_TURN, spd); }  //not used
// void curveRight(int spd) { setMotors(spd, SPEED_TURN); }  //not used