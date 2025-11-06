#define RGBWait 400
#define LDRWait 10
#define LDR 0
#define PrintGray
#define PrintBlack
#define PrintWhite
MeLineFollower lineFinder(PORT_2);

#define RED_THRESHOLD 200                 // Raw RED threshold 
#define RED_ORANGE_THRESHOLD 120          // Raw GREEN threshold to differentiate b/w red and orange
#define PURPLE_GREENBLUE_THRESHOLD 1.2    // Ratio of red and green to differentiate b/w purple and green/blue
#define WHITE_THRESHOLD 200

int red = 0;
int green = 0;
int blue = 0;
char* colours[] = { "Red", "Green", "Blue", "Orange", "Pink" };
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
bool over_half(float color_val) {
  return (color_val > float(255) / 2);
}
// char* classifyColour(int Red, int Green, int Blue) {
//   if (!over_half(Green) && !over_half(Blue)) {
//     return "RED";
//   } else if (!over_half(Red) && !over_half(Green)) {
//     return "BLUE";
//   } else if (!over_half(Blue) && !over_half(Red)) {
//     return "GREEN";
//   } else if (Red > Blue && Green > Blue) {
//     return "ORANGE";
//   } else {
//     return "PINK";
//   }
// }

String classifyColour() {
  int red = colourArray[0], green = colourArray[1], blue = colourArray[2];
  if (red > WHITE_THRESHOLD and green > WHITE_THRESHOLD and blue > WHITE_THRESHOLD) return "WHITE";
  
  if (red > RED_THRESHOLD) { // Either RED or ORANGE
    if (green > RED_ORANGE_THRESHOLD) return "ORANGE";
    else return "RED";
    
  } else {
    float small = min(red, green);
    float large = max(red, green);
    if (small <= 0) small = 1; // Handle division by 0
    float diffGreenRed = large / small;
    if (diffGreenRed < 0) diffGreenRed = -diffGreenRed;

    if (red >= green or diffGreenRed < PURPLE_GREENBLUE_THRESHOLD) 
      return "PINK";
    else {
      if (green > blue) return "GREEN";
      else return "BLUE";
    }
  }
}

String color_sensing() {
  countdown_time(5);
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
  String colour = classifyColour();
  String res = colour;
#ifdef Debug_Color
  Serial.println(res);
#endif
  return res;
}

void countdown_time(int time) {
  for (int i = time; i >= 1; i--) {
    Serial.print(i + " ");
    delay(1000);
  }
  Serial.println();
}

void print_array(float a[]) {
  for (int i = 0; i < 3; ++i) {
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

bool detect_black() {
  int sensorState = lineFinder.readSensors();
  return (sensorState == S1_IN_S2_IN);
}

// void detect_black(){
//   int sensorState = lineFinder.readSensors();
//   if(sensorState == S1_IN_S2_IN){
//     stop();
//     char* res = color_sensing();
//     if(strcmp(res,"Red") == 0){
//       turnLeft();
//     }
//     else if(strcmp(res,"Green") == 0){
//       turnRight();
//     }
//     else if(strcmp(res,"Blue") == 0){
//       doubleRightTurn();
//     }
//     else if(strcmp(res,"Orange")){
//       uTurn();
//     }
//     else{
//       doubleLeftTurn();
//     }
//   }
// }
