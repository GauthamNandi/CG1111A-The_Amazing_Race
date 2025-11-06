#include <MeMCore.h>

// #define Debug_Ultrasonic
// #define Debug_Locomotion
// #define Debug_Color
const int S1 = A2;
const int S2 = A3;

void setup() {
    Serial.begin(9600);
    initialize_PID();

    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    setBalance(); 
}

bool FORWARD = true;
float speedPID;
String g_color = "";

void loop() {
    if (FORWARD) {
        speedPID = calculate_PID();
        moveForward();
        
        if (detect_black()) {
            stopMotor();
            g_color = (String)color_sensing();
            FORWARD = false;
        } 
    } 
    else {
        String s = String(g_color);
        if (s == "RED") turnLeft();
        else if (s == "GREEN") turnRight();
        else if (s == "ORANGE") uTurn();
        else if (s == "Pink") doubleLeftTurn();
        else if (s == "LIGHTBLUE") doubleRightTurn();
        FORWARD = true;
    }
    delay(10);
}
