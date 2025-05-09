#include <math.h>
#include "MPU9250.h"
#include <Wire.h>

#define SPEED 120
#define DELAY 1000
#define LEFT_MOTOR_DIRECTION 12
#define RIGHT_MOTOR_DIRECTION 10
#define LEFT_MOTOR_PWM 11
#define RIGHT_MOTOR_PWM 9
#define FRONT 0
#define BACK 1
#define IMU_ADDRESS 0x68
#define IMU_ERROR_LED A2
#define turnOnLED(p) (digitalWrite(p, HIGH))

#define BIAS_ALPHA 0.975 // 0 to 1, lower --> more responsive

const float hard_iron[3] = {
  -101.99, -40.87, 221.32
};
float yaw = 0.0, gyro_yaw = 0.0, mag_heading, init_mag_heading = 0.0, init_gz = 0.0, prev_yaw = 0.0;

const float soft_iron [3][3] = {
  {  1.00,  0.00, 0.00  },
  {  0.00,  1.00, 0.00  },
  {  0.00,  0.00, 1.00  }
};
const float mag_decl = 4.883;
MPU9250 mpu;

uint8_t read_count = 0;

int previousHeading = 0;

int getHeading() {
  static float hi_cal[3];

  if (read_count < 50)
    read_count += 1;

  if (mpu.update()) {
    float gyroRateZ = mpu.getGyroZ();
    if (read_count >= 10 && read_count < 20)
      if (mpu.update())
        init_gz += gyroRateZ;
    if (read_count == 20) {
      init_gz /= 10;
    }
    if (read_count > 20)
      gyroRateZ -= init_gz;
    gyro_yaw += gyroRateZ * 0.01; // Integration time
    float mag_data[] = {mpu.getMagX(),
                        mpu.getMagY(),
                        mpu.getMagZ()
                       };

    for (uint8_t i = 0; i < 3; i++ ) {
      hi_cal[i] = mag_data[i] - hard_iron[i];
    }
    for (uint8_t i = 0; i < 3; i++ ) {
      mag_data[i] = (soft_iron[i][0] * hi_cal[0]) +
                    (soft_iron[i][1] * hi_cal[1]) +
                    (soft_iron[i][2] * hi_cal[2]);
    }
    mag_heading = -1 * (atan2(mag_data[0], mag_data[1]) * 180) / M_PI;
    mag_heading += mag_decl;
    if (read_count == 20) {
      if (mpu.update()) {
        init_mag_heading = mag_heading;
      }
    }
    mag_heading -= init_mag_heading;
    yaw = BIAS_ALPHA * gyro_yaw + (1 - BIAS_ALPHA) * mag_heading;
    Serial.print("Heading: ");
    Serial.println(int(yaw));
    previousHeading = (int)yaw;
    return (int)yaw;
  } else {
    return previousHeading;
  }
}

void initIMU() {
  MPU9250Setting setting;
  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;//2 4 8 16
  setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;//250 500 1000 2000
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_1000HZ;//125 143 167 200 250 333 500 1000
  setting.gyro_fchoice = 0x03;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;//3600 5 10 20 41 92 184 250
  setting.accel_fchoice = 0x01;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;
  if(!mpu.setup(IMU_ADDRESS, setting)) {
    Serial.println("MPU connection failed.");
    while(1) {}
    turnOnLED(IMU_ERROR_LED);
  }
  mpu.setGyroBias(0.60, -0.92, 1.03);
  while (read_count < 50) {
    getHeading();
  }
}

void stop(unsigned long ms = 0) {
  analogWrite(LEFT_MOTOR_PWM, 0);
  analogWrite(RIGHT_MOTOR_PWM, 0);
  if (ms) delay(ms);
}

void right(uint8_t speed = SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, FRONT);
  digitalWrite(RIGHT_MOTOR_DIRECTION, BACK);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void left(uint8_t speed = SPEED, unsigned long ms = 0) {
  digitalWrite(LEFT_MOTOR_DIRECTION, BACK);
  digitalWrite(RIGHT_MOTOR_DIRECTION, FRONT);
  analogWrite(LEFT_MOTOR_PWM, speed);
  analogWrite(RIGHT_MOTOR_PWM, speed);
  if (ms) delay(ms);
}

void right90() {
  int initialHeading = getHeading();
  int deltaHeading = getHeading() - initialHeading;
  while(abs(deltaHeading) < 115) {
    right();
    deltaHeading = getHeading() - initialHeading;
  }
}

void left90() {
  int initialHeading = getHeading();
  int deltaHeading = getHeading() - initialHeading;
  while(abs(deltaHeading) < 115) {
    left();
    deltaHeading = getHeading() - initialHeading;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LEFT_MOTOR_DIRECTION, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);
  Wire.begin();
  initIMU();
}

void loop() {
  left90();
  stop(DELAY);
  right90();
  stop(DELAY);
}
