const int S1 = A2;
const int S2 = A3;
#define LED 13
#define Debug_Ultrasonic

void initialize(); //PID algorithm

void setup() {
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(LED, OUTPUT);  
  Serial.begin(9600);
  setBalance();  //calibration
  digitalWrite(LED, HIGH);

  initialize();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  adjust_locomotion();
  #ifdef Debug_Ultrasonic
  debug_ulsensor_distance();
  #endif
}
