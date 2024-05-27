#ifndef DEFINES_H
#define DEFINES_H
#include "ir.h"
/* Debugging, Calibration */
#define DEBUG_MODE 0 // 0 -> off, 1 -> on
#define CALIBRATION_PIN A13

/* LEDs */
#define COLOR_ERROR_LED A5
#define TOF_ERROR_LED A6
#define IMU_ERROR_LED A7 // TODO: change to actual pins
#define CENTER_IR_LED A4
#define SILVER_TAPE_LED A3
#define turnOnLED(p) (digitalWrite(p, HIGH))
#define turnOffLED(p) (digitalWrite(p, LOW))

/* IR Sensors */
#define BLACK_THRESHOLD 400
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
const float weights[11] = { -0.5, -0.5, -0.5, -0.5, -0.5, 0, 0.5, 0.5, 0.5, 0.5, 0.5 };
#define reading(a) (analogRead(a) > BLACK_THRESHOLD ? 1 : 0)

/* Color Sensors */
#define LEFT_COLOR 1 
#define RIGHT_COLOR 2
#define LEFT_GREEN 0x46 // F
#define BOTH_GREEN 0x47 // G
#define RIGHT_GREEN 0x48 // H
#define RED 0x52 // R
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
#define SPEED_CHANGE 250
#define s(s) (min(MAX_SPEED, abs(s))) // Speed helper
#define d(s) (s > 0 ? FRONT : BACK) // Direction helper

/* Camera */
extern bool dropped = true;
extern bool inEvacuation = true;
extern bool exitedEvacuation = false;
extern bool wallFollowing = false;
#define RESET_EVACUATION 0
#define PICK 0x50 // P
#define DROP 0x44 // D
#define LEFT 0x4C // L (Turn left while searching for victims)
#define FORWARD 0x46 // F (Go forward while searching for victims)
#define RIGHT 0x52 // R (Turn right while searching for victims)
#define U_TURN 0x55 // U (180deg turn while searching for victims)
#define TURN_360  0x54 // T (360deg turn while searching for victims)
#define VICTIM_LEFT 0x6C // l (Turn left for victim)
#define VICTIM_FORWARD 0x66 // f (Go forward to victim)
#define VICTIM_RIGHT 0x72 // r (Turn right for victim)
#define EXIT 0x45 // E (Exit the evacuation room)
#define WALL_FOLLOW 0x57 // W (Wall follow to the exit)

/* TOFs */
#define RIGHT_TOF_XSHUT 6
#define FRONT_TOF_XSHUT 5
#define RIGHT_TOF_ADDRESS 0x32
#define FRONT_TOF_ADDRESS 0x31

/* IMU */
#define IMU_ADDRESS 0x68
#define IMU_ERROR_FACTOR 1.2778

/* Arm (Servos) */
#define ARM 8
#define GRIPPER 7

/* Obstacle */
#define OBSTACLE A1
#define SEES_OBSTACLE() ((PINF & _BV(PF1)) ? 0 : 1)

#endif