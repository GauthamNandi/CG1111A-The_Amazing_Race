/*
===================================================================
Sensor Measeurements & Calculations
===================================================================
*/

// --- Initalisation ---
#define IR_RECIEVER_PIN A1
#define ULTRASONIC_PIN 12

// --- Constants ---
#define TIMEOUT 5000     
#define SPEED_OF_SOUND 340 

// Returns the Measured Ultrasonic distance
float ultrasonic_distance() {
    digitalWrite(ULTRASONIC_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(ULTRASONIC_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_PIN, LOW);

    pinMode(ULTRASONIC_PIN, INPUT);
    long duration = pulseIn(ULTRASONIC_PIN, HIGH, TIMEOUT);
    return duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100;  // distance in cm
}

// Returns the Measured IR sensor distance
float read_ir_distance() {
    input(0);
    float val_before = analogRead(IR_RECIEVER_PIN);
    delayMicroseconds(100);
    input(3);
    float val_after = analogRead(IR_RECIEVER_PIN); 
    delayMicroseconds(100);

    float voltage = (val_after - val_before) * 5.0 / 1023.0;
    float distance = 20.495 * pow(voltage, -1.1904);
    
    return distance;
}

// Returns Needed Distance
long find_distance() {
    long ultrasonic_dist = ultrasonic_distance();
    
    if (ultrasonic_dist == 0 || ultrasonic_dist > 20) {
        use_ir_sensor = true;
        return read_ir_distance();
    }
    return ultrasonic_dist;
}
