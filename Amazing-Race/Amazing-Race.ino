/*
===================================================================
Main Loop
===================================================================
*/

// DEBUG Definitions
// #define Debug_Movement
// #define Debug_color

// --- Global Variables --- 
bool FORWARD = true;
float speedPID;

// Setup PINs, Serial Monitor Output
void setup() {
    Serial.begin(9600);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(IR_RECIEVER_PIN, INPUT);
    pinMode(ULTRASONIC_PIN, OUTPUT);

    #ifdef Debug_Color
    setBalance();
    #endif 
}

void loop() {
    if (FORWARD) {
        moveForward();
        if (detect_black()) {
            FORWARD = false;
        }
    } 
    else {
        // Detect Colour
        char* s = color_sensing();
        turnLedOn(s);
        delay(100);

        // Execute Commands based on colour
        if (strcmp(s,"Red") == 0) turnLeft();
        else if (strcmp(s,"Green") == 0) turnRight();
        else if (strcmp(s,"Orange") == 0) uTurn();
        else if (strcmp(s,"Pink") == 0) doubleLeftTurn();
        else if (strcmp(s,"Blue") == 0) doubleRightTurn();
        else if (strcmp(s,"White") == 0) celebrate();

        // Change back to FORWARD
        led.setColor(0,0,0); 
        led.show();

        FORWARD = true;
    }
    delay(10);
}
