#include "MeOrion.h"
#define Debug_Color
// #define Debug_Movement
#define IR_RECIEVER_PIN A1
#define ULTRASONIC_PIN 12

#define RGBWait 200
#define LDRWait 10
#define LDR 0

const int S1 = A2;
const int S2 = A3;
bool FORWARD = true;
float speedPID;
char* colours[] = {"Red","Green","Blue","Orange","Pink"};
float colourArray[] = { 0, 0, 0 };
float whiteArray[] = { 462.0, 378.0, 461.0 };
float blackArray[] = { 30.0, 27.0, 43.0 };
float greyDiff[] = { 432.0, 351.0, 418.0 };

// MeBuzzer buzzer;
MeRGBLed led(PORT_3);
void setup() {
    initialize_PID();
    Serial.begin(9600);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(IR_RECIEVER_PIN, INPUT);
    pinMode(ULTRASONIC_PIN, OUTPUT);
    // #ifdef Debug_Color
    // setBalance();
    // #endif 
    // delay(10000);
}

void loop() {
    char* s = color_sensing();
    Serial.println(s);
    // turnLedOn(s);
    // delay(1000);
    // led.setColor(0,0,0);
    // led.show();
    // Serial.println(read_ir());
    // Serial.println(ultrasonic_distance());
    // moveForward();
    // doubleLeftTurn(); delay(1000);
    // doubleRightTurn(); delay(1000);
    // turnLeft(); delay(1000);
    // Serial.println(detect_black());
    // moveForward();
    #ifdef Debug_Movement
    if (FORWARD) {
        // speedPID = calculate_PID();
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
        delay(100);
        led.setColor(0,0,0);
        led.show();

        if (strcmp(s,"Red") == 0) turnLeft();
        else if (strcmp(s,"Green") == 0) turnRight();
        else if (strcmp(s,"Orange") == 0) uTurn();
        else if (strcmp(s,"Pink") == 0) doubleLeftTurn();
        else if (strcmp(s,"Blue") == 0) doubleRightTurn();
        else if (strcmp(s,"White") == 0) celebrate();
        moveForward();
        delay(100);
        FORWARD = true;
    }
    delay(10);
    #endif
}
