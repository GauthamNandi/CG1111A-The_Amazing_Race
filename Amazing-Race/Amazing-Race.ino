#include <MeMCore.h>
const int S1 = A2;
const int S2 = A3;
#define LED 13
// #define Debug_Ultrasonic
#define Debug_Locomotion
// #define Debug_Color
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
  #ifdef Debug_Locomotion
  go_forward();
  detect_black();
  #endif
  #ifdef Debug_Color
  color_sensing();
  #endif
}
