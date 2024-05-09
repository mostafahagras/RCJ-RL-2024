#ifndef CAMERA_H
#define CAMERA_H

#include "defines.h"
#include "motors.h"
#include "TOFs.h"
#include "arm.h"

char message[2];

uint8_t readSerial() {
  Serial3.readBytes(message, 1);
  return message[0];
}

void handleEvacuation(uint8_t message) {
  switch (message) {
    case PICK:
      if(dropped) {
        pick();
        dropped = false;
        Serial3.write("P");
      }
      break;
    case DROP:
      if(!dropped) {
        drop();
        dropped = true;
        Serial3.write("D");
      }
      break;  
    case VICTIM_LEFT:
      left(ROOM_SPEED);
      break;
    case VICTIM_RIGHT:
      right(ROOM_SPEED);
      break;
    case FORWARD:
      forward(ROOM_SPEED);
      break;
    case RIGHT:
      right(80);
      // stop();
      break;
    case LEFT:
      left(ROOM_SPEED);
      break;
    case WALL_FOLLOW:
      Serial3.write("W");
      left();
      delay(750);
      wallFollowing = true;
      wallFollow();
      break;
    case EXIT:
      forward(ROOM_SPEED, 1000);
      inEvacuation = false;
      break;
    default:
      left(50);
      stop();
      break;
  }
}

// if (inEvacuation) {
  //   Serial2.readBytes(message, 1);
  //   Serial.print(message[0]);
  //   if (message[0] == 70) {  // FORWARD
  //     // Serial.println("F");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //   } else if (message[0] == 82) {  // RIGHT
  //     // Serial.println("R");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(LEFT_MOTOR_PWM, 80);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //     analogWrite(RIGHT_MOTOR_PWM, 80);
  //     // delay(100);
  //   } else if (message[0] == 76) {  // LEFT
  //     // Serial.println("L");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //     analogWrite(LEFT_MOTOR_PWM, 80);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(RIGHT_MOTOR_PWM, 80);
  //     // delay(100);
  //   } else if (message[0] == 80) {  // PICK
  //     if (dropped) {
  //       // Serial.println("P");
  //       digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //       digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //       analogWrite(LEFT_MOTOR_PWM, 40);
  //       analogWrite(RIGHT_MOTOR_PWM, 40);
  //       delay(500);
  //       stop(0);
  //       hand.write(35);
  //       arm.write(45);
  //       delay(3000);
  //       arm.write(135);
  //       delay(200);
  //       arm.write(90);
  //       digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //       digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //       analogWrite(LEFT_MOTOR_PWM, 40);
  //       analogWrite(RIGHT_MOTOR_PWM, 40);
  //       delay(1000);
  //       stop(0);
  //       hand.write(120);
  //       delay(1000);
  //       arm.write(135);
  //       delay(1000);
  //       arm.write(90);
  //       dropped = false;
  //     }
  //   } else if (message[0] == 68) {  // DROP
  //     // Serial.println("D");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //     delay(3000);
  //     stop(0);
  //     arm.write(45);
  //     delay(500);
  //     arm.write(90);
  //     hand.write(35);
  //     delay(1000);
  //     arm.write(135);
  //     delay(1500);
  //     arm.write(90);
  //     digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //     delay(1000);
  //     dropped = true;
  //     // arm.write(45);
  //     // delay(3000);
  //     // arm.write(135);
  //     // delay(100);
  //     // arm.write(90);
  //   } else if (message[0] == 66) {  // BACKWARD
  //                                   // Serial.println("B");
  //     digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  //     analogWrite(LEFT_MOTOR_PWM, 40);
  //     digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  //     analogWrite(RIGHT_MOTOR_PWM, 40);
  //   } else {
  //     // Serial.println(message);
  //     //   stop(0);
  //   } 
  // }

#endif