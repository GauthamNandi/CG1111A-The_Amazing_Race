#define TIMEOUT 2000     
#define SPEED_OF_SOUND 340 
#define ULTRASONIC_PIN 12
#define IR_EMITTER_PIN 14
#define IR_RECIEVER_PIN 16

float ultrasonic_distance() {
    pinMode(ULTRASONIC_PIN, OUTPUT);

    digitalWrite(ULTRASONIC_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_PIN, LOW);

    pinMode(ULTRASONIC_PIN, INPUT);
    long duration = pulseIn(ULTRASONIC_PIN, HIGH, TIMEOUT);
    return duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100;  // distance in cm
}

long ir_distance() {
    pinMode(IR_EMITTER_PIN, OUTPUT);
    pinMode(IR_RECIEVER_PIN, INPUT);
    digitalWrite(IR_EMITTER_PIN, HIGH);
    delayMicroseconds(500);

    float total = 0;
    for (int i = 0; i < 10; i++) {
        total += analogRead(IR_RECIEVER_PIN);
        delayMicroseconds(10);
    }
    digitalWrite(IR_EMITTER_PIN, LOW);
    double volt = 5.0 * (total / 10.0) / 1023.0;
    float distance = 27.86 * pow(volt, -1.15) - 3.0;
    distance = 2.0 * distance - 2;
    return distance;
}

long find_distance() {
    if (ultrasonic_distance() > 25) {
        return ir_distance();
    }
    return ultrasonic_distance();
}
