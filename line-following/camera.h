#ifndef CAMERA_H
#define CAMERA_H

#include "defines.h"
#include "motors.h"
#include "TOFs.h"
#include "arm.h"

char message[2];
int searchResult = 0x73;

uint8_t readSerial() {
  Serial3.readBytes(message, 1);
  Serial.print("Message: ");
  Serial.println(message);
  // Serial.println(message[0]);
  return message[0];
}

auto prevMillis = millis();
void search(bool isTriangle = false) {
  int duration = isTriangle ? 4000 : 2000;
  uint16_t rightDistance = distanceRight();
  uint16_t frontDistance = distanceFront();
  Serial.print(rightDistance);
  Serial.print("\tF:");
  Serial.print(frontDistance);
  Serial.print("\tC:");
  prevMillis = millis();
  if (frontDistance < 400) {
    while (millis() - prevMillis < 800 && (cmessage = readSerial()) == (isTriangle ? SEARCH_TRIANGLE : SEARCH)) {
      backward(100);
    }
  } else {
    while (millis() - prevMillis < 800 && (cmessage = readSerial()) == (isTriangle ? SEARCH_TRIANGLE : SEARCH)) {
      forward(100);
    }
  }
  if (cmessage != (isTriangle ? SEARCH_TRIANGLE : SEARCH)) {
    return;
  }
  stop();
  prevMillis = millis();
  if (rightDistance < 400) {
    while (millis() - prevMillis < duration && (cmessage = readSerial()) == (isTriangle ? SEARCH_TRIANGLE : SEARCH)) {
      Serial.println("LEFT");
      left(100);
    }
  } else {
    while (millis() - prevMillis < duration && (cmessage = readSerial()) == (isTriangle ? SEARCH_TRIANGLE : SEARCH)) {
      Serial.println("RIGHT");
      right(100);
    }
  }
  stop();
}

void handleEvacuation(uint8_t message) {
  // Serial.print("message: ");
  // Serial.println(message);
  switch (message) {
    case PICK:
      if (dropped) {
        bool pickResult = pick();
        if (pickResult) {
          Serial3.write("P");
          dropped = false;
          state += 1;
        } else {
          Serial3.write("p");
        }
      }
      break;
    case DROP:
      if (!dropped) {
        drop();
        dropped = true;
        state += 1;
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
      break;
    case LEFT:
      left(ROOM_SPEED);
      break;
    case START_WALL_FOLLOW:
      Serial3.write("W");
      resetArm();
      backward(BASE_SPEED, 200);
      right90();
      stop();
      wallFollowing = true;
      break;
    case WALL_FOLLOW:
      wallFollow();
      break;
    case EXIT:
      forward(ROOM_SPEED, 1000);
      inEvacuation = false;
      break;
    case SEARCH_TRIANGLE:
      search(true);
      break;
    case SEARCH:
      search();
      break;
    default:
      stop();
      break;
  }
}

void checkEvacuationEntrance() {
  backward(BASE_SPEED, 1500);
  stop();
  Serial3.write("a");
  stop(3000);  // wait for camera
  if (Serial3.available()) {
    Serial3.readBytes(message, 1);
    Serial.println(message);
    if (message[0] == TOGGLE_EVACUATION) {
      inEvacuation = true;
      forward(BASE_SPEED, 1000);
      stop();
      Serial3.write(TOGGLE_EVACUATION);
      chooseBus(0);
    }
  } else {
    right90();
    stop(3000);  // wait for camera
    if (Serial3.available()) {
      Serial3.readBytes(message, 1);
      Serial.println(message);
      if (message[0] == TOGGLE_EVACUATION) {
        inEvacuation = true;
        forward(BASE_SPEED, 1000);
        stop();
        Serial3.write(TOGGLE_EVACUATION);
        chooseBus(0);
      }
    } else {
      left180();
      stop(3000);  // wait for camera
      if (Serial3.available()) {
        Serial3.readBytes(message, 1);
        Serial.println(message);
        if (message[0] == TOGGLE_EVACUATION) {
          inEvacuation = true;
          forward(BASE_SPEED, 1000);
          stop();
          Serial3.write(TOGGLE_EVACUATION);
          chooseBus(0);
        }
      } else {
        Serial3.write("a");
        right90();
        stop();
      }
    }
  }
}

#endif