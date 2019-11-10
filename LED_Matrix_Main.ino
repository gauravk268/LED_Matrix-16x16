//Pin connections to the arduino microcontroller
#define SH_CP_1 5   //COLUMN 1-16
#define DS_1 7
#define ST_CP_1 6

#define SH_CP_2 13   //ROW 1-16
#define DS_2 4
#define ST_CP_2 3

#define Vx A0       //Joystick
#define Vy A1
#define Sw A2

int arrColumn[8] = {B10000000, B01000000, B00100000, B00010000, B00001000, B00000100, B00000010, B00000001};
int arrRow[8] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111110};
//int alphabet = {193, 190, 162, 166, 177}  //  @ 0

void LED_On(int ST_CP, int DS, int SH_CP, int value1, int value2) {
  digitalWrite(ST_CP, LOW);
  shiftOut(DS, SH_CP, LSBFIRST, value2);
  shiftOut(DS, SH_CP, LSBFIRST, value1);
  digitalWrite(ST_CP, HIGH);
}

void LED_Off(bool value) {
  //if true turns off all the LED by reversing the polarity
  //i.e. giving HIGH on negative side and LOW on positive side
  //can be used to turn off the LED Matrix

  if (value) {
    LED_On(ST_CP_1, DS_1, SH_CP_1, B00000000, B00000000);
    LED_On(ST_CP_2, DS_2, SH_CP_2, B00000000, B00000000);
  }
}

void LED_Begin() {
  //turns on all the leds to check if all
  //the LEDs are working fine, then turns off

  LED_On(ST_CP_1, DS_1, SH_CP_1, B11111111, B11111111);
  LED_On(ST_CP_2, DS_2, SH_CP_2, B00000000, B00000000);
  delay(1000);
  LED_Off(true);
}

void LED_XY(int x, int y) {
  //turns on the led given the location of the led
  //location given as (row, column) location starting from 1

  x = x - 1; //since numbering in array starts from 0,
  y = y - 1; //and we give values starting from 1

  //conditions for row, turns on the given row
  if (x < 8)
    LED_On(ST_CP_2, DS_2, SH_CP_2, arrRow[x], 255);
  else
    LED_On(ST_CP_2, DS_2, SH_CP_2, 255, arrRow[x-8]);

  //conditions for column, turns on the given column
  if (y < 8)
    LED_On(ST_CP_1, DS_1, SH_CP_1, arrColumn[y], 0);
  else
    LED_On(ST_CP_1, DS_1, SH_CP_1, 0, arrColumn[y-8]);
}

void setup() {
  pinMode(SH_CP_1, OUTPUT);
  pinMode(ST_CP_1, OUTPUT);
  pinMode(DS_1, OUTPUT);

  pinMode(SH_CP_2, OUTPUT);
  pinMode(ST_CP_2, OUTPUT);
  pinMode(DS_2, OUTPUT);

  pinMode(Vx, INPUT);
  pinMode(Vy, INPUT);
  pinMode(Sw, INPUT);

  LED_Begin();
}

void loop() {
//  int x, y, s;
//  x = analogRead(Vx);
//  y = analogRead(Vy);
//  s = analogRead(Sw);
//  
//
//
  for (int i = 1; i < 17; i++) {
    for (int j = 1; j < 17; j++) {
      LED_XY(j, 1);
      delay(100);
    }
  }

  LED_On(ST_CP_1, DS_1, SH_CP_1, 255, 255);
  LED_On(ST_CP_2, DS_2, SH_CP_2, 0, 0);
  delay(1000);
  LED_Off(true);
}
