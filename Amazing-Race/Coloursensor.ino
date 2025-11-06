#define RGBWait 200
#define LDRWait 10 
#define LDR 0
#define PrintGray
#define PrintBlack
#define PrintWhite
MeLineFollower lineFinder(PORT_2);
int red = 0;
int green = 0;
int blue = 0;
int sentivity = 10;
char* colours[] = {"Red","Green","Blue","Orange","Pink"};
float colourArray[] = { 0, 0, 0 };
float whiteArray[] = { 0, 0, 0 };
float blackArray[] = { 0, 0, 0 };
float greyDiff[] = { 0, 0, 0 };

void input(int code) {
  if (code == 0) {
    //TURN RED
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
  } else if (code == 1) {
    //TURN GREEN
    digitalWrite(S1, HIGH);
    digitalWrite(S2, LOW);

  } else if (code == 2) {
    //TURN BLUE
    digitalWrite(S1, LOW);
    digitalWrite(S2, HIGH);
  } else {
    //TURN IR Sensor
    digitalWrite(S1, HIGH);
    digitalWrite(S2, HIGH);
  }
}
bool over_half(float color_val){
  if(color_val>float(255)/2){
    return true;
  }
  else{
    return false;
  }
}
char* classifyColour(int Red, int Green, int Blue){
  if(!over_half(Green-sentivity) && !over_half(Blue-sentivity)){
    return "Red";
  }
  else if(!over_half(Red-sentivity) && !over_half(Green-sentivity)){
    return "Blue";
  }
  else if(!over_half(Blue-sentivity) && !over_half(Red-sentivity)){
    return "Green";
  }
  else if(Red+sentivity>Blue && Green+sentivity>Blue){
    return "Orange";
  }
  else if(Red>=255-3*sentivity && Green>=255-3*sentivity){
    return "White";
  }
  else{
    return "Pink";
  }
}

char* color_sensing() {
  for (int c = 0; c <= 2; c++) {
    input(c);
    delay(RGBWait);
    colourArray[c] = getAvgReading(5);
    colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255.0;
    input(3); 
    delay(RGBWait);
    #ifdef Debug_Color
      Serial.print(int(colourArray[c]));
      Serial.print(",");
    #endif
    delay(LDRWait);
  }
  char *colour = classifyColour(colourArray[0],colourArray[1],colourArray[2]);
  char *res = colour;
  #ifdef Debug_Color
    Serial.print(res);
    Serial.println("");
  #endif
  return res;
}

void countdown_time(int time){
  for(int i=0;i<=time;++i){
    delay(500);
    Serial.print(i);
    Serial.print(" ");
  }
  Serial.println();
}
void print_array(float a[]){
  for(int i=0;i<3;++i){
    Serial.print(a[i]);
    Serial.print(" ");
  } 
  Serial.println();
  delay(500);
}
void setBalance() {
  //set white balance
  Serial.println("Put White Sample For Calibration ...");
  countdown_time(5);
  for (int i = 0; i <= 2; i++) {
    input(i);
    delay(RGBWait);
    whiteArray[i] = getAvgReading(5);
    input(3);
    delay(RGBWait);
  }
  Serial.println("White Array:");
  print_array(whiteArray);
  Serial.println("Put Black Sample For Calibration ...");
  countdown_time(5);
  for (int i = 0; i <= 2; i++) {
    input(i);
    delay(RGBWait);
    blackArray[i] = getAvgReading(5);
    input(3);
    delay(RGBWait);
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }
  Serial.println("Black Array:");
  print_array(blackArray);
  Serial.println("Put colored sheet");
}


int getAvgReading(int times) {
  int reading;
  int total = 0;

  for (int i = 0; i < times; i++) {
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }

  return total / times;
}
void turnLedOn(String s){
  if (s == "Red") led.setColor(255,0,0);
  else if (s == "Green") led.setColor(0,255,0);
  else if (s == "Orange") led.setColor(255,255,0);
  else if (s == "Pink") led.setColor(0,255,255);
  else if (s == "Blue") led.setColor(0,0,255);
  else led.setColor(255,255,255);
  led.show();
}
bool detect_black() {
  int sensorState = lineFinder.readSensors();
  return (sensorState == S1_IN_S2_IN);
}

