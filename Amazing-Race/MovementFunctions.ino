#define TURN_TIME 500
#define TURNING_SPEED 160
#define MOTOR_SPEED 225
#define STRAIGHT_TIME 1000

double kp, ki, kd;
double errsum, lasterr;
unsigned long lsttime;

MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);

void initialize_PID() {
    kp = 18.0;
    ki = 0.0;
    kd = 0.0; 
    lsttime = millis();
}

double calculate_PID() {
    unsigned long now = millis();
    double dt = (now - lsttime) / 1000.0;
    if (dt <= 0) dt = 0.001;

    double setpoint = 8.0;
    double distance = find_distance();
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

void stopMotor() {// Code for stopping motor}
    leftMotor.stop();
    rightMotor.stop();
}

bool irwawy = false;

void moveForward() {// Code for moving forward for some short interval}
    speedPID = -calculate_PID();
    if (irwawy) {
        speedPID = +140;
        irwawy = !irwawy;
    }

    leftMotor.run(MOTOR_SPEED + speedPID);
    rightMotor.run(-MOTOR_SPEED + speedPID);

     Serial.print("PID: "); Serial.print(speedPID);
    Serial.print(" | L: "); Serial.print(MOTOR_SPEED - speedPID);
    Serial.print(" | R: "); Serial.print(-MOTOR_SPEED - speedPID);
    Serial.print(" | Dist: "); Serial.println(find_distance());

}

void helper_turnRight() {// Code for turning right 90 deg}
    leftMotor.run(-TURNING_SPEED);
    rightMotor.run(-TURNING_SPEED);
}

void helper_turnLeft() {// Code for turning right 90 deg}
    leftMotor.run(TURNING_SPEED);
    rightMotor.run(TURNING_SPEED);
}

void turnRight() {
    helper_turnRight();
    delay(TURN_TIME);
    stopMotor();
}

void turnLeft() {
    helper_turnLeft();
    delay(TURN_TIME);
    stopMotor(); 
}

void uTurn() {// Code for u-turn}
    helper_turnRight();
    delay(2 * TURN_TIME);
    stopMotor();
}

void doubleLeftTurn() {// Code for double left turn}
    turnLeft();
    
    moveForward();
    delay(1100);
    stopMotor();

    turnLeft();
}

void doubleRightTurn() {// Code for double right turn}
    turnRight();
    
    moveForward();
    delay(850);
    stopMotor();

    turnRight();
}
