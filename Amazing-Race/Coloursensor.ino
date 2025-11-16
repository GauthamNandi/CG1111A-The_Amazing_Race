/*
===================================================================
Colour Sensor
===================================================================
*/

// --- Initalisation ---
MeLineFollower lineFinder(PORT_2);
MeRGBLed led(PORT_3);

// --- Constants ---
#define RGBWait 200
#define LDRWait 10
#define LDR 0

// --- Global Variables ---
const int S1 = A2;
const int S2 = A3;
float whiteArray[] = { 593.0, 578.0, 474.0 };
float blackArray[] = { 128.0, 127.0, 109.0 };
float greyDiff[] = { 432.0, 351.0, 418.0 };
char* possible_colours[7] = {"White", "Black", "Red", "Orange", "Green", "Pink", "Blue"};
int colour_values[7][3] = { {286, 405, 230},
                            {0, 0, 0},
                            {259, 163, 105},
                            {271, 207, 130},
                            {115, 212, 105},
                            {252, 338, 205},
                            {95, 193, 145},
                          };

// Turns on LEDs or IR Sensor 
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

// Detect Black
bool detect_black() {
  int sensorState = lineFinder.readSensors();
  return (sensorState == S1_IN_S2_IN);
}

// Turn LED on based on Colour
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

// Return euclidian_distance between RGB colours
float euclidian_distance(float r, float g, float b, float r2, float g2, float b2) {
  float dr = r - r2, dg = g - g2, db = b - b2;
  return dr * dr + dg * dg + db * db;
}

// Return the colour closest based on the Euclidian Distance
char* classifyColour(float Red, float Green, float Blue){
  char* return_colour; 
  float min_dist = 1e9;

  for (int i=0; i<7; i++) {
    float cur_dist = euclidian_distance(colour_values[i][0], colour_values[i][1], colour_values[i][2], 
                                        Red, Green, Blue);

    if (cur_dist < min_dist) {
      min_dist = cur_dist;
      return_colour = possible_colours[i];
    }
  }
  return return_colour;
}

// Get Average of Readings
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

// Returns the detected colour
char* color_sensing() {
  float colourArray[] = {0, 0, 0};
  for (int c = 0; c <= 2; c++) {
    input(c);
    delay(RGBWait);
    colourArray[c] = getAvgReading(5);
    colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255.0;
    input(3); 

    delay(RGBWait);
    #ifdef Debug_color
    Serial.print(int(colourArray[c]));
    Serial.print(",");
    #endif
  }
  char *colour = classifyColour(colourArray[0],colourArray[1],colourArray[2]);
  #ifdef Debug_color
  Serial.print(colour);
  Serial.println("");
  #endif
  return colour;
}

#ifdef Debug_color
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

void setBalance() {
  Serial.println("Put White Sample For Calibration ...");
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
#endif
