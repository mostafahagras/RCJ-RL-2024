#ifndef DEFINES_H
#define DEFINES_H
#include "ir.h"
#include <EEPROM.h>
/* Debugging, Calibration */
#define DEBUG_MODE 0  // 0 -> off, 1 -> on
#define CALIBRATION_PIN A13

/* LEDs */
#define IMU_ERROR_LED A2
#define TOF_ERROR_LED A3
#define COLOR_ERROR_LED A4
#define CENTER_IR_LED A5
#define SILVER_TAPE_LED A6
#define turnOnLED(p) (digitalWrite(p, HIGH))
#define turnOffLED(p) (digitalWrite(p, LOW))

/* IR Sensors */
void saveToEEPROM(int value) {
  EEPROM.write(0, (int)(value / 10));
}
int readFromEEPROM() {
  byte value = EEPROM.read(0);
  return value * 10;
}
int BLACK_THRESHOLD = readFromEEPROM();

const uint8_t sensorPins[11] = { 53, 51, 49, 47, 45, A0, 43, 41, 39, 37, 35 };
#define IR0 53
#define IR1 51
#define IR2 49
#define IR3 47
#define IR4 45
#define IR5 A0
#define IR6 43
#define IR7 41
#define IR8 39
#define IR9 37
#define IR10 35
#define THRESHOLD_INCREMENT_INTERRUPT 18
#define THRESHOLD_DECREMENT_INTERRUPT 19

void INCREMENT_THRESHOLD() {
  BLACK_THRESHOLD = min(BLACK_THRESHOLD + 10, 1000);
  Serial.println(BLACK_THRESHOLD);
  saveToEEPROM(BLACK_THRESHOLD);
}
void DECREMENT_THRESHOLD() {
  BLACK_THRESHOLD = max(BLACK_THRESHOLD - 10, 20);
  Serial.println(BLACK_THRESHOLD);
  saveToEEPROM(BLACK_THRESHOLD);
}
const float weights[11] = { -0.5, -0.5, -0.5, -0.5, -0.5, 0, 0.5, 0.5, 0.5, 0.5, 0.5 };
#define reading(a) (analogRead(a) > BLACK_THRESHOLD ? 1 : 0)

/* Color Sensors */
#define LEFT_COLOR 1
#define RIGHT_COLOR 2
#define LEFT_GREEN 0x46   // F
#define BOTH_GREEN 0x47   // G
#define RIGHT_GREEN 0x48  // H
#define RED 0x52          // R
#define NO_COLOR 0
#define GREEN 1
#define RED 2
#define GREEN_THRESHOLD 30
#define RED_THRESHOLD 30

/* Motors */
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 0
#define BACK 1
#define MAX_SPEED 254
#define BASE_SPEED 120
#define ROOM_SPEED 80
#define SPEED_CHANGE 125
#define s(s) (min(MAX_SPEED, abs(s)))  // Speed helper
#define d(s) (s > 0 ? FRONT : BACK)    // Direction helper

/* Camera */
extern bool dropped = true;
extern bool inEvacuation = false;
extern bool wallFollowing = false;
extern uint8_t cmessage = 0x53;
extern uint8_t state = 0;
extern bool exitedEvacuation = false;
#define RESET_EVACUATION 0
#define PICK 0x50               // P (Pick the victim)
// #define PICK_FAILED 0x70        // p (Picking the victim failed)
#define DROP 0x44               // D (Drop the victim)
#define LEFT 0x4C               // L (Turn left while searching for victims)
#define FORWARD 0x46            // F (Go forward while searching for victims)
#define RIGHT 0x52              // R (Turn right while searching for victims)
#define U_TURN 0x55             // U (180deg turn while searching for victims)
#define TURN_360 0x54           // T (360deg turn while searching for victims)
#define VICTIM_LEFT 0x6C        // l (Turn left for victim)
#define VICTIM_FORWARD 0x66     // f (Go forward to victim)
#define VICTIM_RIGHT 0x72       // r (Turn right for victim)
#define WALL_FOLLOW 0x57        // W (Wall follow to the exit)
#define START_WALL_FOLLOW 0x77  // w (Start wall following)
#define EXIT 0x45               // E (Exit the evacuation room)
#define SEARCH 0x53             // S (Search for victims in the evacuation room)
#define SEARCH_TRIANGLE 0x73    // s (Search for triangles in the evacuation room)
#define TOGGLE_EVACUATION 0x74  // t (Toggle inEvacuation)

/* TOFs */
#define RIGHT_TOF_XSHUT 6
#define FRONT_TOF_XSHUT 5
#define RIGHT_TOF_ADDRESS 0x32
#define FRONT_TOF_ADDRESS 0x31

/* IMU */
#define IMU_ADDRESS 0x68
#define IMU_ERROR_FACTOR 1.2778

/* Arm (Servos) */
#define ARM 7
#define GRIPPER 8

/* Obstacle */
#define OBSTACLE A1
#define SEES_OBSTACLE() ((PINF & _BV(PF1)) ? 0 : 1)

#endif