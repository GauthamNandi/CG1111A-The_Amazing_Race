/*
===================================================================
mBot Robot Movement Control
===================================================================
*/

// --- Initalisation ---
MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);

// --- Constants ---
#define TURN_TIME 500
#define TURNING_SPEED 160
#define MOTOR_SPEED 225
#define STRAIGHT_TIME 1000
#define DOUBLE_LEFT_STRAIGHT_TIME 1100
#define DOUBLE_RIGHT_STRAIGHT_TIME 850

// --- Global Variables --- 
bool use_ir_sensor = false;

// Calculate (P)ID
double calculate_PID() {
    double kp = 18.0
    double setpoint = 8.0;
    double distance = find_distance();
    double error = setpoint - distance;

    double pid = kp * error;

    if (pid > 255) pid = 255;
    if (pid < -255) pid = -255;

    return pid;
}

// Move Forward with PID correction
void moveForward() {
    speed_PID = -calculate_PID();
    if (use_ir_sensor) { // Uses the IR sensor, hence speed_PID should be negative
        speed_PID = -speed_PID; 
        use_ir_sensor = false;
    }
    leftMotor.run(MOTOR_SPEED + speed_PID);
    rightMotor.run(-MOTOR_SPEED + speed_PID);

    #ifdef Debug_Movement
    Serial.print("PID: "); Serial.print(speedPID);
    Serial.print(" | L: "); Serial.print(MOTOR_SPEED - speedPID);
    Serial.print(" | R: "); Serial.print(-MOTOR_SPEED - speedPID);
    Serial.print(" | Dist: "); Serial.println(find_distance());
    #endif
}

// Stops Motors
void stopMotor() { 
    leftMotor.stop();
    rightMotor.stop();
}

// Sets motors to spin RIGHT
void helper_turnRight() { 
    leftMotor.run(-TURNING_SPEED);
    rightMotor.run(-TURNING_SPEED);
}

// Sets motors to spin LEFT
void helper_turnLeft() { 
    leftMotor.run(TURNING_SPEED);
    rightMotor.run(TURNING_SPEED);
}

// Executes a 90-degree RIGHT turn
void turnRight() {
    helper_turnRight();
    delay(TURN_TIME);
    stopMotor();
}

// Executes a 90-degree LEFT turn
void turnLeft() {
    helper_turnLeft();
    delay(TURN_TIME);
    stopMotor(); 
}

// Executes a 180-degree RIGHT turn
void uTurn() {
    helper_turnRight();
    delay(2 * TURN_TIME);
    stopMotor();
}

// Executes a double RIGHT turn
void doubleRightTurn() {
    turnRight();

    moveForward();
    delay(DOUBLE_RIGHT_STRAIGHT_TIME);
    stopMotor();

    turnRight();
}

// Executes a double LEFT turn
void doubleLeftTurn() {
    turnLeft();
    
    moveForward();
    delay(DOUBLE_LEFT_STRAIGHT_TIME);
    stopMotor();

    turnLeft();
}
