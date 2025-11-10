#define TIMEOUT 5000     
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
    //pinMode(IR_EMITTER_PIN, OUTPUT);
    pinMode(IR_RECIEVER_PIN, INPUT);
    input(3); // digitalWrite(IR_EMITTER_PIN, HIGH);
    delayMicroseconds(500);

    float total = 0;
    for (int i = 0; i < 10; i++) {
        total += analogRead(IR_RECIEVER_PIN);
        delayMicroseconds(10);
    }
    
    input(0); // digitalWrite(IR_EMITTER_PIN, LOW);
    // double volt = 5.0 * (total / 10.0) / 1023.0;
    // float distance = 27.86 * pow(volt, -1.15) - 3.0;
    // distance = 2.0 * distance - 2;
    float adc = total / 10.0;
    float distance = 270.0 * pow(adc, -1.1);
    if (distance < 1) distance = 1;
    if (distance > 20) distance = 20;
    return distance;
}

long find_distance() {
    // Serial.println(ir_distance());
    // long u_dist = ultrasonic_distance();
    // if (u_dist == 0 || u_dist > 20) {
    //     return ir_distance();
    // }
    long u_dist = ultrasonic_distance() - ir_distance();

    return u_dist;
}
