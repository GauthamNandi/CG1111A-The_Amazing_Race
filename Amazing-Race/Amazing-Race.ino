#include <MeMCore.h>
const int S1 = A2;
const int S2 = A3;
#define LED 13

MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
uint8_t motorSpeed = 200;
// #define Debug_Ultrasonic
// #define Debug_Locomotion
// #define Debug_Color
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
  delay(1000);
  turn_left();
  turn_right();
  turn_U();
  #ifdef Debug_Locomotion
  go_forward();
  detect_black();
  #endif
  #ifdef Debug_Color
  color_sensing();
  #endif
}
