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
    }
    turnOnLED(TOF_ERROR_LED);
    delay(1000);
    turnOffLED(TOF_ERROR_LED);
    delay(1000);
    turnOnLED(TOF_ERROR_LED);
    delay(1000);
    turnOffLED(TOF_ERROR_LED);
    delay(1000);
  } else {
    if (DEBUG_MODE) { Serial.println("Found Right TOF"); }
  }
  delay(10);
  digitalWrite(FRONT_TOF_XSHUT, HIGH);
  delay(10);
  if (!frontTOF.begin(FRONT_TOF_ADDRESS)) {
    if (DEBUG_MODE) {
      Serial.println("Failed to boot front VL53L0X");
      while (1) {}
    }
    turnOnLED(TOF_ERROR_LED);
    delay(1000);
    turnOffLED(TOF_ERROR_LED);
  } else {
    if (DEBUG_MODE) { Serial.println("Found Front TOF"); }
  }
}

uint16_t distanceRight() {
  rightTOF.rangingTest(&measureRight, false);
  if (DEBUG_MODE) {
    // Serial.print(measureRight.RangeMilliMeter);
  }
  return measureRight.RangeMilliMeter;
}

uint16_t distanceFront() {
  frontTOF.rangingTest(&measureFront, false);
  if (DEBUG_MODE) {
    // Serial.print(measureFront.RangeMilliMeter);
  }
  return measureFront.RangeMilliMeter;
}

unsigned long startMillis = millis();
char msg[2];

void wallFollow() {
  uint16_t rightDistance = distanceRight();
  uint16_t frontDistance = distanceFront();
  if (DEBUG_MODE) {
    Serial.print("Right TOF: ");
    Serial.print(rightDistance);
    Serial.print("\tFront TOF: ");
    Serial.print(frontDistance);
    Serial.print("\t\n");
  }
  if (rightDistance > 500 && frontDistance > 1200) {
    stop();
    const int8_t position = -_IR0() - _IR1() - _IR2() - _IR3() - _IR4() + _IR6() + _IR7() + _IR8() + _IR9() + _IR10();
    if (position > 0) {
      right(ROOM_SPEED, position * 150);
    } else if (position < 0) {
      left(ROOM_SPEED, -position * 150);
    }
    backward(ROOM_SPEED, 2000);
    forward(ROOM_SPEED);
    startMillis = millis();
    while (millis() - startMillis <= 2500) {
      if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 5) {
        inEvacuation = false;
        exitedEvacuation = true;
        break;
      }
      // if (SILVER_1() && SILVER_2() && SILVER_3()) {
      //   backward(BASE_SPEED, 750);
      //   left90();
      //   forward(BASE_SPEED, 1000);
      //   inEvacuation = true;
      //   break;
      // } else {
      //   inEvacuation = false;
      // }
    }
    if (inEvacuation) {
      backward(ROOM_SPEED, 2000);
      left90();
      left90();
      forward(ROOM_SPEED, 500);
      if (distanceRight() > 300) {
        right90();
        stop();
        forward(BASE_SPEED, 1000);
      }
    }
  } else if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 4) {
    if (frontDistance < 500 || abs(getPitch()) >= 5) {
      backward(ROOM_SPEED);
      delay(200);
      left(ROOM_SPEED);
      delay(200);
    } else {
      forward(ROOM_SPEED, 300);
    }
  } else if (rightDistance <= 225 && frontDistance <= 150) {
    left(ROOM_SPEED);
    delay(100);
  } else if (rightDistance > 500 && frontDistance <= 1200) {
    startMillis = millis();
    forward(ROOM_SPEED);
    while (millis() - startMillis < 500 || distanceFront() > 60) {}
    stop();
    right90();
    backward(ROOM_SPEED, 500);
    startMillis = millis();
    forward();
    while (millis() - startMillis <= 1000) {
      if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 5) {
        inEvacuation = false;
        exitedEvacuation = true;
        break;
      }
    }
    if (!inEvacuation) {
      backward(BASE_SPEED, 750);
    } else {
      backward(BASE_SPEED, 500);
      left180();
      forward(BASE_SPEED, 500);
    }
  } else if (rightDistance <= 225 && rightDistance >= 30) {
    forward(ROOM_SPEED);
  } else if (rightDistance < 30) {
    drive(ROOM_SPEED / 2, ROOM_SPEED);
  } else if (rightDistance > 225) {
    if (_IR0() || _IR1() || _IR2() || _IR3() || _IR4() || _IR6() || _IR7() || _IR8() || _IR9() || _IR10() || reading(IR5)) {
      stop();
      const int8_t position = -_IR0() - _IR1() - _IR2() - _IR3() - _IR4() + _IR6() + _IR7() + _IR8() + _IR9() + _IR10();
      if (position > 0) {
        right(ROOM_SPEED, position * 150);
      } else if (position < 0) {
        left(ROOM_SPEED, -position * 150);
      }
      backward(ROOM_SPEED, 2000);
      forward(ROOM_SPEED);
      startMillis = millis();
      while (millis() - startMillis <= 2500) {
        if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 5) {
          inEvacuation = false;
          exitedEvacuation = true;
          break;
        }
        // if (SILVER_1() && SILVER_2() && SILVER_3()) {
        //   backward(BASE_SPEED, 750);
        //   left90();
        //   forward(BASE_SPEED, 1000);
        //   inEvacuation = true;
        //   break;
        // } else {
        //   inEvacuation = false;
        // }
      }
      if (inEvacuation) {
        backward(ROOM_SPEED, 2000);
        left90();
        left90();
        forward(ROOM_SPEED, 500);
        if (distanceRight() > 300) {
          right90();
          stop();
          forward(BASE_SPEED, 1000);
        }
      }
    } else {
      drive(ROOM_SPEED, ROOM_SPEED / 2);
    }
  } else {
    forward(ROOM_SPEED);
  }
  // if (SILVER_1() && SILVER_2() && SILVER_3()) {
  //   backward(BASE_SPEED, 500);
  //   left90();
  //   left90();
  //   forward(ROOM_SPEED, 500);
  //   if (distanceRight() > 300) {
  //     right90();
  //     stop();
  //     forward(BASE_SPEED, 1000);
  //   }
  // }
  // else if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 2 && frontDistance > 500) {
  //   backward(BASE_SPEED, 250);
  //   inEvacuation = false;
  // }
}

void _wallFollow() {
  int pitch = getPitch();
  if (pitch <= -5) {  // To avoid climbing the wall and triangles
    backward(ROOM_SPEED, 200);
    left(ROOM_SPEED, 200);
  }
  uint16_t rightDistance = distanceRight();
  uint16_t frontDistance = distanceFront();
  if (_IR0() || _IR1() || _IR2() || _IR3() || _IR4() || _IR6() || _IR7() || _IR8() || _IR9() || _IR10() && abs(pitch) < 5) {
    forward(ROOM_SPEED, 200);
    pitch = getPitch();
    if (pitch <= -5) {
      backward(ROOM_SPEED, 200);
      left(ROOM_SPEED, 200);
    } else {
      forward(ROOM_SPEED, 200);
      if (distanceRight() - rightDistance > 50) {
        backward(ROOM_SPEED, 1000);
        forward(ROOM_SPEED);
        startMillis = millis();
        while (millis() - startMillis < 1500) {
          if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 5) {
            inEvacuation = false;
            exitedEvacuation = true;
            return;
          }
        }
        if (inEvacuation) {
          stop();
          const int8_t position = -_IR0() - _IR1() - _IR2() - _IR3() - _IR4() + _IR6() + _IR7() + _IR8() + _IR9() + _IR10();
          if (position > 0) {
            right(ROOM_SPEED, position * 150);
          } else if (position < 0) {
            left(ROOM_SPEED, -position * 150);
          }
          backward(ROOM_SPEED, 1500);
          left180();
          forward(ROOM_SPEED, 500);
          if (distanceRight() > 300) {
            right90();
            stop();
            forward(BASE_SPEED, 1000);
          }
        }
      } else {
        backward(ROOM_SPEED, 200);
        inEvacuation = false;
        exitedEvacuation = true;
        return;
      }
    }
    // stop(1000);
  } else if (frontDistance < 1200 && rightDistance >= 500) {
    startMillis = millis();
    forward(ROOM_SPEED);
    while (millis() - startMillis < 500 || distanceFront() > 60) {}
    stop();
    right90();
    backward(ROOM_SPEED, 500);
    startMillis = millis();
    forward();
    while (millis() - startMillis <= 1000) {
      if (_IR0() + _IR1() + _IR2() + _IR3() + _IR4() + reading(IR5) + _IR6() + _IR7() + _IR8() + _IR9() + _IR10() > 5) {
        inEvacuation = false;
        exitedEvacuation = true;
        break;
      }
    }
    if (!inEvacuation) {
      backward(BASE_SPEED, 750);
    } else {
      backward(BASE_SPEED, 500);
      left180();
      forward(BASE_SPEED, 500);
    }
  } else if (rightDistance <= 225 && frontDistance <= 150) {
    left(ROOM_SPEED, 100);
  } else if (rightDistance < 30) {
    drive(ROOM_SPEED / 2, ROOM_SPEED);
  } else if (rightDistance < 225) {
    forward(ROOM_SPEED);
  } else if (rightDistance >= 225) {
    drive(ROOM_SPEED, ROOM_SPEED / 2);
  } else {
    stop();
  }
}

#endif