#define TIMEOUT 5000     
#define SPEED_OF_SOUND 340 

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
float read_ir(){
    input(0);
    float val_before = analogRead(IR_RECIEVER_PIN);
    delayMicroseconds(100);
    input(3);
    float val_after = analogRead(IR_RECIEVER_PIN); 
    delayMicroseconds(100);
    return (val_after - val_before) * 5.0 / 1023.0;
}

double ir_distance() {
    double total = 0.0;
    for (int i = 0; i < 10; i++) {
        total+=(double)read_ir();
    }
    input(0);
    total= (double)total / 10.0;
    // 4) Distance model derived from your calibration
    // distance ≈ 270 * adc^-1.1   (cm)
    float distance = 270.0 * pow(total, -1.1);

    // 5) Clamp range (avoid crazy readings)
    if (distance < 1) distance = 1;
    if (distance > 20) distance = 20;

    Serial.println(distance);
    return distance;
    // return pow(total/321.8,-2.469);
}

long find_distance() {
    long u_dist = ultrasonic_distance();
    if (u_dist == 0 || u_dist > 20) {
        if (read_ir()>1.3) irwawy = true;
        return 9.0;
    }
    return u_dist;
}
