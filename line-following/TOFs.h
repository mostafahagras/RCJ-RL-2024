#ifndef TOFS_H
#define TOFS_H

#include "Adafruit_VL53L0X.h"
#include "defines.h"
#include "motors.h"
#include "mux.h"

Adafruit_VL53L0X rightTOF = Adafruit_VL53L0X();
Adafruit_VL53L0X frontTOF = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measureRight;
VL53L0X_RangingMeasurementData_t measureFront;

void initTOFs() {
  digitalWrite(RIGHT_TOF_XSHUT, LOW);
  digitalWrite(FRONT_TOF_XSHUT, LOW);
  delay(10);
  digitalWrite(RIGHT_TOF_XSHUT, HIGH);
  digitalWrite(FRONT_TOF_XSHUT, HIGH);
  delay(10);
  digitalWrite(RIGHT_TOF_XSHUT, HIGH);
  digitalWrite(FRONT_TOF_XSHUT, LOW);
  if (!rightTOF.begin(RIGHT_TOF_ADDRESS)) {
    if (DEBUG_MODE) {
      Serial.println("Failed to boot right VL53L0X");
      while (1) {}
    } else {
      turnOnLED(TOF_ERROR_LED);
    }
  }
  delay(10);
  digitalWrite(FRONT_TOF_XSHUT, HIGH);
  delay(10);
  if (!frontTOF.begin(FRONT_TOF_ADDRESS)) {
    if (DEBUG_MODE) {
      Serial.println("Failed to boot front VL53L0X");
      while (1) {}
    } else {
      turnOnLED(TOF_ERROR_LED);
    }
  }
}

uint16_t distanceRight() {
  rightTOF.rangingTest(&measureRight, false);
  if(DEBUG_MODE) {
    Serial.print(measureRight.RangeMilliMeter);
  }
  return measureRight.RangeMilliMeter;
}

uint16_t distanceFront() {
  frontTOF.rangingTest(&measureFront, false);
  if(DEBUG_MODE) {
    Serial.print(measureFront.RangeMilliMeter);
  }
  return measureFront.RangeMilliMeter;
}

unsigned long startMillis = millis();
char msg[2];

void wallFollow() {
  uint16_t rightDistance = distanceRight();
  uint16_t frontDistance = distanceFront();
  if (DEBUG_MODE) {
    Serial.print("Right TOF:");
    Serial.print(rightDistance);
    Serial.print(" Front TOF:");
    Serial.println(frontDistance);
  }
  if(frontDistance > 8000 && rightDistance > 500) {
    backward(BASE_SPEED, 1500);
    stop();
    startMillis = millis();
    while(millis() - startMillis <= 5000) {
      Serial3.readBytes(msg, 1);
      if(msg[0] == EXIT) {
        forward(ROOM_SPEED, 1000);
        inEvacuation = false;
        exitedEvacuation = true;
      } 
    }
    if(inEvacuation) {
      left(ROOM_SPEED, 2000);
      forward(ROOM_SPEED, 2500);
    }
  } else if (digitalRead(sensorPins[0]) + digitalRead(sensorPins[1]) + digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) + digitalRead(sensorPins[5]) + digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) + digitalRead(sensorPins[9]) + digitalRead(sensorPins[10]) >= 5) {
    if(frontDistance < 8000) {
      backward(ROOM_SPEED);
      delay(200);
      left(ROOM_SPEED);
      delay(200);
    } else {
    backward(BASE_SPEED, 1000);
        drive(BASE_SPEED, BASE_SPEED/2, 1000);      
      inEvacuation = false;
        exitedEvacuation = true;
       // forward(BASE_SPEED,200);
    }

  } 
  // else if(frontDistance > 500) {
  //   backward(ROOM_SPEED, 500);
  //   startMillis = millis();
  //   while(millis - startMillis <= 5000) {
  //     Serial3.readBytes(msg, 1);
  //     if(msg[0] == EXIT) {
  //       forward(ROOM_SPEED, 1000);
  //       inEvacuation = false;
  //       exitedEvacuation = true;
  //     }
  //   }
  // } 
  else if (frontDistance <= 150 && rightDistance <= 300) {
    left(ROOM_SPEED);
    delay(100);
  } else if (rightDistance > 500 && frontDistance <= 8000) {
    // while (
    //   !(digitalRead(sensorPins[0]) + digitalRead(sensorPins[1]) + digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) + digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) == 0 && digitalRead(sensorPins[9]) && digitalRead(sensorPins[10])) 
    //   &&
    //   !(digitalRead(sensorPins[0]) && digitalRead(sensorPins[1]) && digitalRead(sensorPins[2]) + digitalRead(sensorPins[3]) + digitalRead(sensorPins[4]) + digitalRead(sensorPins[6]) + digitalRead(sensorPins[7]) + digitalRead(sensorPins[8]) + digitalRead(sensorPins[9]) + digitalRead(sensorPins[10]) == 0)
    // ) {
    // if(digitalRead(sensorPins[10])) {
    //   turn_right = true;
    // }
    drive(ROOM_SPEED, 0, 4000);
    backward(BASE_SPEED, 1000);
    stop();
    startMillis = millis();
    while(millis() - startMillis <= 5000) {
      stop();
      Serial3.readBytes(msg, 1);
      if(msg[0] == EXIT) {
        forward(ROOM_SPEED, 1000);
        inEvacuation = false;
        exitedEvacuation = true;
        break;
      } else {
       while(distanceRight() > 100 || distanceFront() < 300){
        drive(BASE_SPEED/2,-BASE_SPEED/2);      }
      //drive(BASE_SPEED/2,-BASE_SPEED/2,500);
      stop(5000);
      drive(BASE_SPEED/2,-BASE_SPEED/2,500);
    }

    }
    // }
  } else if (rightDistance <= 300 && rightDistance >= 50) {
    forward(ROOM_SPEED);
  } else if (rightDistance < 50) {
    drive(ROOM_SPEED/2, ROOM_SPEED);
  } else if (rightDistance > 300) {
    drive(ROOM_SPEED, ROOM_SPEED/2);
  } else {
    forward(ROOM_SPEED);
  }
}

#endif