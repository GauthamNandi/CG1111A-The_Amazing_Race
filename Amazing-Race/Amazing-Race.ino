#include <MeMCore.h>
// #define Debug_Color
// #define Debug_Movement
const int S1 = A2;
const int S2 = A3;
bool FORWARD = true;
float speedPID;
String g_color = "";

void setup() {
    initialize_PID();
    Serial.begin(9600);
    #ifdef Debug_Color
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    setBalance();
    #endif 
    // turnLeft();
    // turnRight();
    // uTurn();
    doubleLeftTurn();
    // doubleRightTurn();
}

void loop() {
    // moveForward();
    #ifdef Debug_Movement
    if (FORWARD) {
        Serial.println("ngu1");
        speedPID = calculate_PID();
        moveForward();
        
        if (detect_black()) {
            stopMotor();
            g_color = (String)color_sensing();
            FORWARD = false;
        } 
    } 
    else {
        Serial.println("ngu2");
        String s = String(g_color);
        if (s == "Red") turnLeft();
        else if (s == "Green") turnRight();
        else if (s == "Orange") uTurn();
        else if (s == "Pink") doubleLeftTurn();
        else if (s == "Blue") doubleRightTurn();
        FORWARD = true;
    }
    delay(10);
    #endif
}
