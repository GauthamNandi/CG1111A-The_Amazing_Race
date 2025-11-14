MeLineFollower lineFinder(PORT_2);
int red = 0;
int green = 0;
int blue = 0;
int sentivity = 20;
int LIMIT = 100;

int arr[7][3] = { {286, 405, 230},
                  {0, 0, 0},
                  {259, 163, 105},
                  {271, 207, 130},
                  {115, 212, 105},
                  {252, 338, 205},
                  {95, 193, 145},
                };
char* colrrr[7] = {"White", "Black", "Red", "Orange", "Green", "Pink", "Blue"};

float euclidian_distance(float r, float g, float b, float r2, float g2, float b2) {
  float dr = r - r2, dg = g - g2, db = b - b2;
  return dr * dr + dg * dg + db * db;
}

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
  else if (strcmp(s,"Orange")==0) led.setColor(255,165,0);
  else if (strcmp(s,"Pink")==0) led.setColor(231,84,128);
  else if (strcmp(s,"Blue")==0) led.setColor(0,0,255);
  else if(strcmp(s,"Yellow") == 0) led.setColor(255,255,0);
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
  for (int i = time; i > 0; --i) {
    Serial.print(i);
    Serial.print(",");
    delay(100);
  }
  Serial.println();
}

bool over_limit(float color_val) {
  return (color_val > LIMIT);
}

char* classifyColour(float Red, float Green, float Blue){
  char* ret = NULL; float min_dist = 1e9;
  if (max(Red, max(Green, Blue)) > 380) return "White";


  for (int i=0; i<7; i++) {
    float cur_dist = euclidian_distance(arr[i][0], arr[i][1], arr[i][2], 
                                      Red, Green, Blue);

    if (cur_dist < min_dist) {
      min_dist = cur_dist;
      ret = colrrr[i];
    }
  }
  return ret;
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
  delay(3000);
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
