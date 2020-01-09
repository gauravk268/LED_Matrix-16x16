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

bool matrixLED[16][16]={
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false},
  {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}
};

int alphabetsRow[][5]={
  {193, 190, 162, 166, 177},  //  @ 0
  {131, 237, 238, 237, 131},  //  A 00
  {201, 182, 182, 182, 128},  //  B 01
  {221, 190, 190, 190, 193},  //  C 02
  {193, 190, 190, 190, 128},  //  D 4
  {190, 182, 182, 182, 128}, //  E  5
  {254, 246, 246, 246, 128}, //  F  6
  {142, 190, 190, 174, 193}, //  G  7
  {128, 247, 247, 247, 128}, //  H  8
  {255, 190, 128, 190, 255}, //  I  9
  {192, 191, 191, 191, 223}, //  J  10
  {190, 221, 235, 247, 128}, //  K  11
  {191, 191, 191, 191, 128}, //  L  12
  {128, 253, 243, 253, 128}, //  M  13
  {128, 239, 247, 251, 128}, //  N  14
  {193, 190, 190, 190, 193}, //  O  15
  {241, 238, 238, 238, 128}, //  P  16
  {161, 190, 174, 222, 193}, //  Q  17
  {184, 214, 230, 246, 128}, //  R  18
  {205, 182, 182, 182, 217}, //  S  19
  {254, 254, 128, 254, 254}, //  T  20
  {192, 191, 191, 191, 192}, //  U  21
  {224, 223, 191, 223, 224}, //  V  22
  {128, 223, 231, 223, 128}, //  W  23
  {156, 235, 247, 235, 156}, //  X  24
  {252, 251, 131, 251, 252}, //  Y  25
  {188, 186, 182, 174, 158}, //  Z  26
  {193, 186, 182, 174, 193}, // 0 27
  {255, 191, 128, 189, 255}, // 1 28
  {185, 182, 182, 174, 157}, // 2 29
  {204, 178, 182, 190, 222}, // 3 30
  {239, 228, 237, 135, 231}, // 4 31
  {198, 186, 186, 186, 216}, // 5 32
  {198, 182, 182, 181, 195}, // 6 33
  {252, 142, 246, 250, 254}, // 7 34
  {201, 182, 182, 182, 201}, // 8 35
  {225, 214, 182, 182, 185}, // 9 36
  {255, 255, 255, 255, 255} //    37
  //  {}, //  ]
  //  {}, //  \
  //  {}, //  [
  //  {}, //  ^
  //  {}, //  _
  //  {}, //  !
  //  {}, //  "
  //  {}, //  #
  //  {}, //  $
  //  {}, //  %
  //  {}, //  &
  //  {}, //  '
  //  {}, //  (
  //  {}, //  )
  //  {}, //  *
  //  {}, //  +
  //  {}, //  ,
  //  {}, //  -
  //  {}, //  .
  //  {}, // /
  //  {}, // :
  //  {}, // ;
  //  {}, // <
  //  {}, // >
  //  {}, // =
  //  {}, // ?
};

void LED_On(int ST_CP, int DS, int SH_CP, int value1, int value2){
  digitalWrite(ST_CP, LOW);
  shiftOut(DS, SH_CP, MSBFIRST, value1);
  shiftOut(DS, SH_CP, MSBFIRST, value2);
  digitalWrite(ST_CP, HIGH);
}

void LED_Off(bool value) {
  //if true turns off all the LED by reversing the polarity
  //i.e. giving HIGH on negative side and LOW on positive side
  //can be used to turn off the LED Matrix

  if (value) {
    LED_On(ST_CP_1, DS_1, SH_CP_1, B00000000, B00000000);
    LED_On(ST_CP_2, DS_2, SH_CP_2, B11111111, B11111111);
  }
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
    LED_On(ST_CP_2, DS_2, SH_CP_2, 255, arrRow[x - 8]);

  //conditions for column, turns on the given column
  if (y < 8)
    LED_On(ST_CP_1, DS_1, SH_CP_1, arrColumn[y], 0);
  else
    LED_On(ST_CP_1, DS_1, SH_CP_1, 0, arrColumn[y - 8]);
}

void updateLED(int x, int y, bool value){
  x = x-1;
  y = y-1;

  matrixLED[x][y] = value;
}

void setup(){
  pinMode(SH_CP_1, OUTPUT);
  pinMode(ST_CP_1, OUTPUT);
  pinMode(DS_1, OUTPUT);

  pinMode(SH_CP_2, OUTPUT);
  pinMode(ST_CP_2, OUTPUT);
  pinMode(DS_2, OUTPUT);

  pinMode(Vx, INPUT);
  pinMode(Vy, INPUT);
  pinMode(Sw, INPUT);

  Serial.begin(9600);
}

void loop(){
  int x, y, s, count=0;
  x = analogRead(Vx);
  y = analogRead(Vy);
  s = analogRead(Sw);

  int i, j;
  for(i=1; i<17; i++){
    for(j=1; j<17; j++){
      if(matrixLED[i+1][j+1] == true){
        LED_XY(i, j);
        Serial.println(count++);
      }
      delay(1);
    }
  }
}
