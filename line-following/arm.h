#ifndef ARM_H
#define ARM_H

#include <Servo.h>
#include "defines.h"
#include "motors.h"

Servo arm, gripper;

void resetArm() {
  arm.write(250);
}

void initServos() {
  arm.attach(ARM);
  gripper.attach(GRIPPER);
  resetArm();
}

void raiseArm() {
  arm.write(135);
  delay(2000);
  // arm.write(135);
  // delay(1000);
  // arm.write(90);
}

bool raiseArmToVerifyPick() {
  arm.write(172);
  delay(1000);
  for (byte i = 0; i < 100; i++) {
    if(!digitalRead(A7)) {
      raiseArm();
      return true;
    }
  }
  raiseArm();
  return false;
}

void lowerArmToPick() {
  arm.write(25);
  delay(2000);
  // arm.write(45);
  // delay(3000);
  // arm.write(135);
  // delay(150);
  // arm.write(90);
}

void lowerArmToDrop() {
  arm.write(50);
  delay(2000);
  // arm.write(45);
  // delay(1000);
  // arm.write(90);
}

void closeGripper() {
  gripper.write(60);
  delay(2000);
}

void openGripper() {
  gripper.write(0);
  delay(2000);
}

bool pick() {
  backward(ROOM_SPEED, 400);
  stop();
  openGripper();
  lowerArmToPick();
  forward(100, 1200);
  stop();
  closeGripper();
  backward(BASE_SPEED, 500);
  stop();
  if(state != 4) {
    return raiseArmToVerifyPick();
  }
  raiseArm();
  return true;
}

void drop() {
  auto startMillis = millis();
  while(!(_IR0() || _IR1() || _IR2() || _IR3() || _IR4() || _IR6() || _IR7() || _IR8() || _IR9() || _IR10()) && millis() - startMillis < 2000) {
    forward(ROOM_SPEED);
  }
  backward(ROOM_SPEED, 500);
  stop();
  lowerArmToDrop();
  openGripper();
  delay(1000);
  backward(ROOM_SPEED, 500);
  stop();
  raiseArm();
  left180();
}

#endif