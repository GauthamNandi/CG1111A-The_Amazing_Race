#include "MeMCore.h"

MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
uint8_t motorSpeed = 200;

#define TIMEOUT 2000
#define SPEED_OF_SOUND 340
#define ULTRASONIC 12

long dist();

void forward(int pid) {
  // ✅ Fixed: Inverted correction so robot turns AWAY from wall
  if (pid < 0) {
     leftMotor.run(motorSpeed - pid);
    rightMotor.run(-motorSpeed - pid);
  } else {
    leftMotor.run(motorSpeed + pid);
    rightMotor.run(-motorSpeed + pid);
  }


  Serial.print("PID: "); Serial.print(pid);
  Serial.print(" | L: "); Serial.print(motorSpeed + pid);
  Serial.print(" | R: "); Serial.print(-motorSpeed - pid);
  Serial.print(" | Dist: "); Serial.println(dist());
}

double kp, ki, kd;
double errsum, lasterr;
unsigned long lsttime;

void initialize() {
  kp = 4.0;
  ki = 0.0;
  kd = 2; 
  lsttime = millis();
}

int calcpid() {
  unsigned long now = millis();
  double dt = (now - lsttime) / 1000.0;
  if (dt <= 0) dt = 0.001;

  long setpoint = 11;
  double distance = dist();
  double error = setpoint - distance;

  errsum += error * dt;
  double dErr = (error - lasterr) / dt;
  double pid = kp * error + ki * errsum + kd * dErr;

  if (pid > 255) pid = 255;
  if (pid < -255) pid = -255;

  lasterr = error;
  lsttime = now;

  // return 0;
  return pid;
}
void adjust_locomotion(){
  double pid = calcpid();
  forward(pid);
  delayMicroseconds(50);
}