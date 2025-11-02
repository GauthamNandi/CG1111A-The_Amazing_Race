#define TIMEOUT 2000     
#define SPEED_OF_SOUND 340 
#define ULTRASONIC 12
long dist() {
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);
  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);
  return duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100;  // distance in cm
}

void debug_ulsensor_distance() {
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);
  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);
  if (duration > 0) {
    Serial.print("distance(cm) = ");
    Serial.println(duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100);
  } else {
    Serial.println("out of range");
  }
  delay(500);
}
