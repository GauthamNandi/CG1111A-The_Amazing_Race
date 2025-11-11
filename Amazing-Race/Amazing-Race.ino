#include "MeOrion.h"
#define Debug_Color
#define Debug_Movement
#define IR_RECIEVER_PIN A1
#define ULTRASONIC_PIN 12

#define RGBWait 400
#define LDRWait 10
#define LDR 0

const int S1 = A2;
const int S2 = A3;
bool FORWARD = true;
float speedPID;
char* colours[] = {"Red","Green","Blue","Orange","Pink"};
float colourArray[] = { 0, 0, 0 };
float whiteArray[] = { 0, 0, 0 };
float blackArray[] = { 0, 0, 0 };
float greyDiff[] = { 0, 0, 0 };

// MeBuzzer buzzer;
MeRGBLed led(PORT_3);
void setup() {
    initialize_PID();
    Serial.begin(9600);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(IR_RECIEVER_PIN, INPUT);
    pinMode(ULTRASONIC_PIN, OUTPUT);
    #ifdef Debug_Color
    setBalance();
    #endif 
    // delay(10000);
}

void loop() {
    #ifdef Debug_Movement
    if (FORWARD) {
        speedPID = calculate_PID();
        moveForward();
        
        if (detect_black()) {
            stopMotor();
            FORWARD = false;
        } 
    } 
    else {
        delay(100);
        char* s = color_sensing();
        Serial.println(s);
        turnLedOn(s);
        delay(1000);
        led.setColor(0,0,0);
        led.show();

        if (strcmp(s,"Red") == 0) turnLeft();
        else if (strcmp(s,"Green") == 0) turnRight();
        else if (strcmp(s,"Orange") == 0) uTurn();
        else if (strcmp(s,"Pink")) doubleLeftTurn();
        else if (strcmp(s,"Blue")) doubleRightTurn();
        else if (strcmp(s,"White")) celebrate();
        moveForward();
        delay(100);
        FORWARD = true;
    }
    delay(10);
    #endif
}
