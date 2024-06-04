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

void lowerArmToPick() {
  arm.write(20);
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
  gripper.write(110);
  delay(2000);
}

void openGripper() {
  gripper.write(0);
  delay(2000);
}

void pick() {
  backward(ROOM_SPEED, 400);
  stop();
  openGripper();
  lowerArmToPick();
  forward(BASE_SPEED, 1000);
  stop();
  closeGripper();
  backward(BASE_SPEED, 500);
  stop();
  raiseArm();
}

void drop() {
  forward(ROOM_SPEED, 2000);
  backward(ROOM_SPEED, 750);
  stop();
  lowerArmToDrop();
  openGripper();
  delay(1000);
  raiseArm();
  deadEnd();
}

#endif