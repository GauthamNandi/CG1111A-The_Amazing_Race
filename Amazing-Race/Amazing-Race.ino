#include "MeOrion.h"
// #define Debug_Color
#define Debug_Movement
const int S1 = A2;
const int S2 = A3;
bool FORWARD = true;
float speedPID;
String g_color = "";
MeRGBLed led(PORT_3);
void setup() {
    initialize_PID();
    Serial.begin(9600);
    #ifdef Debug_Color
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    setBalance();
    #endif 
}

void loop() {
    find_distance();
    // doubleLeftTurn();
    // delay(5000);
    // doubleRightTurn();
    // turnLeft();
    // turnRight();
    // delay(1000);
    // uTurn();

    // delay(1000);
    // speedPID = calculate_PID();
    // moveForward();

    // // #ifdef Debug_Movement
    // if (FORWARD) {
    //     speedPID = calculate_PID();
    //     moveForward();
        
    //     if (detect_black()) {
    //         stopMotor();
    //         Serial.println("HERE");
    //         g_color = (String)color_sensing();
    //         FORWARD = false;
    //     } 
    // } 
    // else {
    //     String s = String(g_color);
    //     turnLedOn(s);
    //     delay(1000);
    //     led.setColor(0,0,0);
    //     led.show();

    //     if (s == "Red") turnLeft();
    //     else if (s == "Green") turnRight();
    //     else if (s == "Orange") uTurn();
    //     else if (s == "Pink") doubleLeftTurn();
    //     else if (s == "Blue") doubleRightTurn();
    //     else if (s == "White") return;
    //     moveForward();
    //     delay(100);
    //     FORWARD = true;
    // }
    // delay(10);
    // #endif
}
