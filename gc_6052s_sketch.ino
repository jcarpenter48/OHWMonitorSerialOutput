//GLOBAL VARIABLES
//int allSegments[] = {6, 7, 8, 9, 10, 12, 13, 15, 16, 17, 18, 19, 20, 21}; //gc 6502s is broken up into these segments, see documentation for mapping
//int allSegmentsCount = 14;
int incomingByte = 0; // for incoming serial data
boolean cathodeOneEnable = false, cathodeTwoEnable = false; //cathodeOne = A5, cathodeTwo = A4, colon = A2
/*
 * dig1: 6, 7, 8, 9
 * dig2: 9, 10, 12, 13
 * dig3: 15, 16, 17, 18
 * dig4: 18, 19, 20, 21
 */
//boolean disabledSegmentsA[14]; //all enabled/disabled segments will be stored here to iterate through
//boolean disabledSegmentsB[14];
boolean enabledSegmentsA[14]; //15th segment is the colon symbol, pin 22 on the display, will default to enabled
boolean enabledSegmentsB[14];
int dig1 = 3;
int dig2 = 4;
int dig3 = 5;
int dig4 = 0;
int counter = 0;

//HELPER METHODS
void cathodeOne(boolean enable) {
  if(enable) {
    cathodeTwo(false);
    pinMode(A5, OUTPUT); //set digital pin 1 to output to control it
    digitalWrite(A5, LOW);
    return;
  }
  pinMode(A5, INPUT); //we don't want to send voltage to GNDPIN1 on the screen
  //so we basically take digital pin 1 back to floating instead
}
void cathodeTwo(boolean enable) {
  if(enable) {
    cathodeOne(false); //only one cathode at a time
    pinMode(A4, OUTPUT); //set digital pin 2 to output to control it
    digitalWrite(A4, LOW);
    return;
  }
  pinMode(A4, INPUT); //we don't want to send voltage to GNDPIN2 on the screen
  //so we basically take digital pin 2 back to floating instead
}

void drawDigits() {
   //cathodeOneEnable = false, cathodeTwoEnable = false;
   cathodeOne(0); cathodeTwo(0);
   //standard: first line cathodes, second line high pulls, third break
   //left to right: dig 1, dig2, dig3, dig4
   //int allSegments[] = {6, 7, 8, 9, 10, 12, 13, 15, 16, 17, 18, 19, 20, 21};
   
   //enabledSegmentsA[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   //enabledSegmentsB[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //reset these to false arrays
  memset(enabledSegmentsA, 0, sizeof(enabledSegmentsA));
  memset(enabledSegmentsB, 0, sizeof(enabledSegmentsA));
   
   switch (dig3) {
       //all indices are Segment# - 8
        case 1:
          // 16a and 17a
          enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          break;
        case 2:
          // 15a, 16a, 16b, 17b, 18b
          enabledSegmentsA[7] = 1; enabledSegmentsA[8] = 1;
          enabledSegmentsB[8] = 1; enabledSegmentsB[9] = 1; enabledSegmentsB[10] = 1;
          break;
        case 3:
          // 15a, 16a, 17a, 16b, 17b
          enabledSegmentsA[7] = 1; enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[8] = 1; enabledSegmentsB[9] = 1;
          break;
        case 4:
          // 16a, 17a, 15b, 16b
          enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[7] = 1; enabledSegmentsB[8] = 1;
          break;
        case 5:
          // 15a, 17a, 15b, 16b, 17b 
          enabledSegmentsA[7] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[7] = 1; enabledSegmentsB[8] = 1; enabledSegmentsB[9] = 1;
          break;
        case 6:
          // 15a, 17a, 15b, 16b, 17b, 18b
          enabledSegmentsA[7] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[7] = 1; enabledSegmentsB[8] = 1; enabledSegmentsB[9] = 1; enabledSegmentsB[10] = 1;
          break;
        case 7:
          // 15a, 16a, 17a
          enabledSegmentsA[7] = 1; enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          break;
        case 8:
          // 15a, 16a, 17a, 15b, 16b, 17b, 18b
          enabledSegmentsA[7] = 1; enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[7] = 1; enabledSegmentsB[8] = 1; enabledSegmentsB[9] = 1; enabledSegmentsB[10] = 1;
          break; 
        case 9:
          // 15a, 16a, 17a, 15b, 16b, 17b
          enabledSegmentsA[7] = 1; enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[7] = 1; enabledSegmentsB[8] = 1; enabledSegmentsB[9] = 1;
          break;  
        default:
          //case 0
          // 15a, 16a, 17a, 15b, 17b, 18b
          enabledSegmentsA[7] = 1; enabledSegmentsA[8] = 1; enabledSegmentsA[9] = 1;
          enabledSegmentsB[7] = 1; enabledSegmentsB[9] = 1; enabledSegmentsB[10] = 1;
          // statements
          break;
      }
   switch (dig4) {
       //all indices are Segment# - 8
        case 1:
          // 19b and 20b
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1;
          break;
        case 2:
          // 18a, 19a, 20a, 19b, 21b
          enabledSegmentsA[10] = 1; enabledSegmentsA[11] = 1; enabledSegmentsA[12] = 1;
          enabledSegmentsB[11] = 1; enabledSegmentsB[13] = 1;
          break;
        case 3:
          // 19a, 20a, 19b, 20b, 21b
          enabledSegmentsA[11] = 1; enabledSegmentsA[12] = 1;
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1;  enabledSegmentsB[13] = 1;
          break;
        case 4:
          // 19a, 21a, 19b, 20b, 21b
          enabledSegmentsA[11] = 1; enabledSegmentsA[13] = 1;
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1;  enabledSegmentsB[13] = 1;
          break;
        case 5:
          // 19a, 20a, 21a, 20b, 21b
          enabledSegmentsA[11] = 1; enabledSegmentsA[12] = 1; enabledSegmentsA[13] = 1;
          enabledSegmentsB[12] = 1;  enabledSegmentsB[13] = 1;
          break;
        case 6:
          // 18a, 19a, 20a, 21a, 20b, 21b
          enabledSegmentsA[10] = 1; enabledSegmentsA[11] = 1; enabledSegmentsA[12] = 1; enabledSegmentsA[13] = 1;
          enabledSegmentsB[12] = 1; enabledSegmentsB[13] = 1;
          break;
        case 7:
          // 19b, 20b, 21b
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1; enabledSegmentsB[13] = 1;
          break;
        case 8:
          // 18a, 19a, 20a, 21a, 19b, 20b, 21b
          enabledSegmentsA[10] = 1; enabledSegmentsA[11] = 1; enabledSegmentsA[12] = 1; enabledSegmentsA[13] = 1;
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1; enabledSegmentsB[13] = 1;
          break; 
        case 9:
          // 19a, 20a, 21a, 19b, 20b, 21b
          enabledSegmentsA[11] = 1; enabledSegmentsA[12] = 1; enabledSegmentsA[13] = 1;
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1; enabledSegmentsB[13] = 1;
          break;  
        default:
          //case 0
          // 18a, 20a, 21a, 19b, 20b, 21b
          enabledSegmentsA[10] = 1; enabledSegmentsA[12] = 1; enabledSegmentsA[13] = 1;
          enabledSegmentsB[11] = 1; enabledSegmentsB[12] = 1; enabledSegmentsB[13] = 1;
          // statements
          break;
      }
   cathodeOne(1); //enable segment A
   for (int i = 0; i < 14; i++) {
     boolean temp = enabledSegmentsA[i];
     if (temp) {
      digitalWrite(i, HIGH); //if enabled, pull high
     } else {
      digitalWrite(i, LOW); //if not enabled, pull low
     }
   }
   delay(10); //wait a moment before flashing to Cathode 2's enabled pins
   cathodeTwo(1); //enable segment B
   for (int i = 0; i < 14; i++) {
     boolean temp = enabledSegmentsB[i];
     if (temp) {
      digitalWrite(i, HIGH); //if enabled, pull high
     } else {
      digitalWrite(i, LOW); //if not enabled, pull low
     }
   }
   delay(10); //wait a moment before flashing to Cathode 2's enabled pins
   //cathodeOne(cathodeOneEnable);
   //cathodeTwo(cathodeTwoEnable);
}
//MAIN METHODS
void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); //gc 6
  pinMode(3, OUTPUT); //gc 7
  pinMode(4, OUTPUT); //gc 8
  pinMode(5, OUTPUT); //gc 9
  pinMode(6, OUTPUT); //gc 10
  pinMode(7, OUTPUT); //gc 12
  pinMode(8, OUTPUT); //gc 13
  pinMode(9, OUTPUT); //gc 15
  pinMode(10, OUTPUT); //gc 16
  pinMode(11, OUTPUT); //gc 17
  pinMode(12, OUTPUT); //gc 18
  pinMode(13, OUTPUT); //gc 19
  pinMode(A0, OUTPUT); //gc 20
  pinMode(A1, OUTPUT); //gc 21
  pinMode(A2, OUTPUT); //gc 22
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    // read the incoming CPU Temp:
    cathodeOne(0); cathodeTwo(0);
    counter = Serial.parseInt();
    dig3 = (int)(counter / 10);
    dig4 = (int)(counter % 10);
  }
  /*
   * for (int i = 0; i < allSegmentsCount; i++) {
   *  digitalWrite(allSegments[i], LOW); //pull all segments to GND and turn off
   *  //we will re-enable those that need to be enabled shortly
   * }
   */
  drawDigits();
  /*counter++;
   if (counter > 20) {
     counter = 0;
     dig4++;
     if(dig4 > 9) {
      dig3++;
      if(dig3 > 9) {
        dig3 = 0;
      }
      dig4 = 0;
     }
   }*/
}
