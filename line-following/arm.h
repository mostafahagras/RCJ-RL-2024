#ifndef ARM_H
#define ARM_H

#include <Servo.h>
#include "defines.h"
#include "motors.h"

Servo arm, gripper;

void initServos() {
  arm.attach(ARM);
  gripper.attach(GRIPPER);
}

void raiseArm() {
  arm.write(135);
  delay(1000);
  arm.write(90);
}

void lowerArmToPick() {
  arm.write(45);
  delay(3000);
  arm.write(135);
  delay(150);
  arm.write(90);
}

void lowerArmToDrop() {
  arm.write(45);
  delay(1000);
  arm.write(90);
}

void closeGripper() {
  gripper.write(120);
}

void openGripper() {
  gripper.write(35);
}

void pick() {
  backward(ROOM_SPEED, 400);
  stop();
  openGripper();
  lowerArmToPick();
  forward(BASE_SPEED, 1000);
  stop();
  closeGripper();
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
}

#endif