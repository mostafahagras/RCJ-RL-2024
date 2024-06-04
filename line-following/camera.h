#ifndef CAMERA_H
#define CAMERA_H

#include "defines.h"
#include "motors.h"
#include "TOFs.h"
#include "arm.h"

char message[2];

uint8_t readSerial() {
  Serial3.readBytes(message, 1);
  Serial.print("Message: ");
  Serial.println(message);
  // Serial.println(message[0]);
  return message[0];
}
auto prevMillis = millis();
int search(bool isTriangle = false) {
  int duration = isTriangle ? 2000 : 1000;
  int m = 0;
  Serial.print("Search: R:");
  uint16_t rightDistance = distanceRight();
  uint16_t frontDistance = distanceFront();
  Serial.print(rightDistance);
  Serial.print("\tF:");
  Serial.print(frontDistance);
  Serial.print("\tC:");
  prevMillis = millis();
  if (frontDistance < 400) {
    while (millis() - prevMillis < duration && (m = readSerial()) == SEARCH) {
      backward(ROOM_SPEED);
    }
  } else {
    while (millis() - prevMillis < duration && (m = readSerial()) == SEARCH) {
      forward(ROOM_SPEED);
    }
  }
  if (m != SEARCH) {
    return m;
  }
  stop();
  prevMillis = millis();
  if (rightDistance < 400) {
    while (millis() - prevMillis < duration && (m = readSerial()) == SEARCH) {
      left(ROOM_SPEED);
    }
  } else {
    while (millis() - prevMillis < duration && (m = readSerial()) == SEARCH) {
      right(ROOM_SPEED);
    }
  }
  if (m != SEARCH) {
    return m;
  }
  stop();
  return 0;
}

void handleEvacuation(uint8_t message) {
  switch (message) {
    case PICK:
      if (dropped) {
        pick();
        dropped = false;
        Serial3.write("P");
      }
      break;
    case DROP:
      if (!dropped) {
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
    case START_WALL_FOLLOW:
      Serial3.write("W");
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
    case SEARCH:
      int result = search();
      if (result) {
        handleEvacuation(result);
      }
      break;
    case SEARCH_TRIANGLE:
      int result2 = search(true);
      if(result2) {
        handleEvacuation(result);
      }
      break;
    default:
      left(60);
      // stop();
      break;
  }
}

#endif