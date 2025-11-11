MeLineFollower lineFinder(PORT_2);
int red = 0;
int green = 0;
int blue = 0;
int sentivity = 20;
int LIMIT = 100;
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

bool detect_black() {
  int sensorState = lineFinder.readSensors();
  return (sensorState == S1_IN_S2_IN);
}

void turnLedOn(char* s){
  if (strcmp(s,"Red") == 0) led.setColor(255,0,0);
  else if (strcmp(s,"Green")==0) led.setColor(0,255,0);
  else if (strcmp(s,"Orange")==0) led.setColor(255,255,0);
  else if (strcmp(s,"Pink")==0) led.setColor(0,255,255);
  else if (strcmp(s,"Blue")==0) led.setColor(0,0,255);
  else if(strcmp(s,"Yellow") == 0) led.setColor(0,255,255);
  else led.setColor(255,255,255);
  led.show();
}

void print_array(float a[]) {
  for (int i = 0; i < 3; ++i) {
    Serial.print(a[i]);
    Serial.print(" ");
 }
  Serial.println();
  delay(500);
}

void countdown_time(int time) {
  for (int i = time; i >0; --i) {
    Serial.print(i);
    Serial.print(",");
    delay(500);
  }
  Serial.println();
}

#ifdef Debug_Color
bool over_limit(float color_val) {
  return (color_val > LIMIT);
}

char* classifyColour(float Red, float Green, float Blue){
  if(!over_limit(Green) && !over_limit(Blue)){
    return "Red";
  }
  else if(!over_limit(Red) && over_limit(Blue) && Blue > Green + 1.5*sentivity){
    return "Blue";
  }
  else if(!over_limit(Blue-sentivity) && !over_limit(Red)){
    return "Green";
  }
  else if(!over_limit(Blue-sentivity)){
    return "Orange";
  }
  else if(Green+1.5 * sentivity>255.0 || Blue + 1.5 *  sentivity > 255.0 || Red + 1.5 * sentivity > 255.0){
    return "White";
  }
  else{
    return "Pink";
  }
}


char* color_sensing() {
  turnLedOn("White");
  countdown_time(3);
  led.setColor(0,0,0);
  led.show();
  for (int c = 0; c <= 2; c++) {
    input(c);
    delay(RGBWait);
    colourArray[c] = getAvgReading(5);
    colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255.0;
    input(3); 
    delay(RGBWait);
    Serial.print(int(colourArray[c]));
    Serial.print(",");
    delay(LDRWait);
  }
  char *colour = classifyColour(colourArray[0],colourArray[1],colourArray[2]);
  Serial.print(colour);
  Serial.println("");
  return colour;
}


void setBalance() {
  //set white balance
  Serial.println("Put White Sample For Calibration ...");
  // turnLedOn("White");
  // countdown_time(5);
  // led.setColor(0,0,0);
  // led.show();
  for (int i = 0; i <= 2; i++) {
    input(i);
    delay(RGBWait);
    whiteArray[i] = getAvgReading(5);
    input(3);
    delay(RGBWait);
  }
  Serial.println("White Array:");
  print_array(whiteArray);
  turnLedOn("White");
  Serial.println("Put Black Sample For Calibration ...");
  turnLedOn("White");
  countdown_time(5);
  led.setColor(0,0,0);
  led.show();
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
}
#endif
