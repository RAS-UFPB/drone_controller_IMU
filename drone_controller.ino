#include <Wire.h>
#include <MPU6050.h>

#define MOTOR_1 11
#define MOTOR_2 10
#define MOTOR_3 5
#define MOTOR_4 9

int flag_time = millis();

MPU6050 imu;
int pitch, roll;
float yaw;

void setup() {
  Serial.begin(9600);

  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);
  pinMode(MOTOR_4, OUTPUT);

  while (!imu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Não foi possível encontrar um sensor MPU6050 válido, verifique a ligação!");
    delay(500);
  }

  imu.calibrateGyro();
  imu.setThreshold(1);
}

void loop() {
    if(millis() - flag_time >= 300) {
      Vector accel_normalized = imu.readNormalizeAccel();
      Vector gyro_normalized = imu.readNormalizeGyro();

      get_roll_pitch_yaw(accel_normalized, gyro_normalized);

      Serial.print("Pitch = ");
      Serial.print(pitch);
      Serial.print("\tRoll = ");
      Serial.print(roll);
      Serial.print("\tYaw = ");
      Serial.print(yaw);
      Serial.print("\n");
    }

    if(pitch < -8 && roll < -8) {
      analogWrite(MOTOR_1, 100);
      analogWrite(MOTOR_2, 100);
      analogWrite(MOTOR_3, 100);
      analogWrite(MOTOR_4, 255);
    } else if(pitch > 8 && roll < -8) {
      analogWrite(MOTOR_1, 100);
      analogWrite(MOTOR_2, 255);
      analogWrite(MOTOR_3, 100);
      analogWrite(MOTOR_4, 100);
    } else if(pitch < -8 && roll > 8) {
      analogWrite(MOTOR_1, 100);
      analogWrite(MOTOR_2, 100);
      analogWrite(MOTOR_3, 255);
      analogWrite(MOTOR_4, 100);
    } else if(pitch > 8 && roll > 8) {
      analogWrite(MOTOR_1, 255);
      analogWrite(MOTOR_2, 100);
      analogWrite(MOTOR_3, 100);
      analogWrite(MOTOR_4, 100);
    } else if(pitch > 8) {
      analogWrite(MOTOR_1, 255);
      analogWrite(MOTOR_2, 255);
      analogWrite(MOTOR_3, 100);
      analogWrite(MOTOR_4, 100);
    } else if(pitch < -8) {
      analogWrite(MOTOR_1, 100);
      analogWrite(MOTOR_2, 100);
      analogWrite(MOTOR_3, 255);
      analogWrite(MOTOR_4, 255);
    } else if(roll > 8) {
      analogWrite(MOTOR_1, 255);
      analogWrite(MOTOR_2, 100);
      analogWrite(MOTOR_3, 255);
      analogWrite(MOTOR_4, 100);
    } else if(roll < -8) {
      analogWrite(MOTOR_1, 100);
      analogWrite(MOTOR_2, 255);
      analogWrite(MOTOR_3, 100);
      analogWrite(MOTOR_4, 255);
    } else {
      analogWrite(MOTOR_1, 100);
      analogWrite(MOTOR_2, 100);
      analogWrite(MOTOR_3, 100);
      analogWrite(MOTOR_4, 100);
    }
}

void get_roll_pitch_yaw(Vector accel_normalized, Vector gyro_normalized){
  roll = (atan2(accel_normalized.YAxis, accel_normalized.ZAxis) * 180) / 3.14;
  pitch = -(atan2(accel_normalized.XAxis, sqrt(pow(accel_normalized.YAxis, 2) + pow(accel_normalized.ZAxis, 2))) * 180) / 3.14;

  if(gyro_normalized.ZAxis > 1 || gyro_normalized.ZAxis < -1) {
    gyro_normalized.ZAxis /= 100;
    yaw += gyro_normalized.ZAxis;
  }

  if(yaw < 0) {
    yaw += 360;    
  } else if(yaw > 359) {
    yaw -= 360;
  }
}