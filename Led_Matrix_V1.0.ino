#include <EEPROM.h>

#define DS_pin1  7            //for columns (anode)
#define SH_CP_pin1  5
#define ST_CP_pin1  6

#define DS_pin2  4            //for rows (cathode)
#define SH_CP_pin2  13
#define ST_CP_pin2  3

#define Vx A0                 //Joystick
#define Vy A1
#define Sw A2

#define UP 0
#define DOWN 2
#define LEFT 1
#define RIGHT 3            //joystick values for snake
#define MORE 700
#define LESS 300


// Global Constant
int f ;
int s ;
uint8_t snakeHigh = 130 ;
uint8_t bricksHigh = 169 ;
int snakeHighScore = 0 ;
int bricksHighScore = 0 ;
int score = 0;
// Global constants for BLOCK BREAKER
int x = 8, y = 14, right = 1, up = 1;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int arrColumn[8] = {B10000000, B01000000, B00100000, B00010000, B00001000, B00000100, B00000010, B00000001};
int arrRow[8] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111110};
int alphabetsRow[][5] = {
  {193, 190, 162, 166, 177}, //  @  0
  {131, 237, 238, 237, 131}, //  A  1
  {201, 182, 182, 182, 128}, //  B  2
  {221, 190, 190, 190, 193}, //  C  3
  {193, 190, 190, 190, 128}, //  D  4
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
  {193, 186, 182, 174, 193}, //  0  27
  {255, 191, 128, 189, 255}, //  1  28
  {185, 182, 182, 174, 157}, //  2  29
  {204, 178, 182, 190, 222}, //  3  30
  {239, 228, 237, 135, 231}, //  4  31
  {198, 186, 186, 186, 216}, //  5  32
  {198, 182, 182, 181, 195}, //  6  33
  {252, 142, 246, 250, 254}, //  7  34
  {201, 182, 182, 182, 201}, //  8  35
  {225, 214, 182, 182, 185}, //  9  36
  {255, 255, 255, 255, 255} //      37
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


unsigned int a[16] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
//values for a single column at a time, starting from the leftmost column to the rightmost column


////////////////////////SNAKE////////////////
struct vec2 {
  int x, y;
};


int snakeLength = 1, direc = UP;
vec2 *positions = new vec2[10];
vec2 *pos = positions;
vec2 applePos = {0, 0};

///////////////////////SNAKE//////////////

// Don't edit this function, working properly
void shift_resistor_receive_data(int data1, int data2) {
  int i = 0;
  for (i = 0; i < 16; i++) {
    if ( data1 & 0b00000000000000001) {
      digitalWrite(DS_pin1, HIGH);
    }
    else {
      digitalWrite(DS_pin1, LOW);
    }
    data1 = data1 >> 1;
    digitalWrite(SH_CP_pin1, HIGH);
    digitalWrite(SH_CP_pin1, LOW);
  }
  digitalWrite(ST_CP_pin1, HIGH);
  digitalWrite(ST_CP_pin1, LOW);

  for (i = 0; i < 16; i++) {
    if ( data2 & 0b1000000000000000) {
      digitalWrite(DS_pin2, HIGH);
    }
    else {
      digitalWrite(DS_pin2, LOW);
    }
    data2 = data2 << 1;
    digitalWrite(SH_CP_pin2, HIGH);
    digitalWrite(SH_CP_pin2, LOW);
  }
  digitalWrite(ST_CP_pin2, HIGH);
  digitalWrite(ST_CP_pin2, LOW);
}


void hexToArray(int n[], bool arr[][16]) {
  uint16_t temp;
  for (uint8_t i = 0; i < 16; i++) {
    temp = n[i];
    for (uint8_t j = 0; j < 16; j++) {
      arr[i][15 - j] = bool(0x0001 & temp);
      temp = temp >> 1;
    }
  }
}

// Don't edit this function, working properly
void LED_On(int ST_CP, int DS, int SH_CP, int value1, int value2) {
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
    LED_On(ST_CP_pin1, DS_pin1, SH_CP_pin1, B00000000, B00000000);
    LED_On(ST_CP_pin2, DS_pin2, SH_CP_pin2, B00000000, B00000000);
  }
}


void LED_Column(int y) {
  //turns on the given column

  y = y - 2;
  if (y >= 8)
    LED_On(ST_CP_pin1, DS_pin1, SH_CP_pin1, arrColumn[y - 8], 0);
  else
    LED_On(ST_CP_pin1, DS_pin1, SH_CP_pin1, 0, arrColumn[y]);
}


void LED_PrintOneValue(int num) {
  //Prints one character in 7x5 matrix one by one


  for (int i = 1; i < 13 ; i++) {
    for (int j = 1; j < 6; j++) {
      LED_Column(i + j);
      LED_On(ST_CP_pin2, DS_pin2, SH_CP_pin2, alphabetsRow[num + 1][j - 1], 255);
      delay(10);
      LED_Off(true);
    }
  }

}


void printtwodigitnumber(int num) {
  f = num % 10 ;
  s = num / 10 ;
  LED_PrintOneValue(26 + s);
  delay(15);
  LED_PrintOneValue(26 + f);
  delay(15) ;
}


void print_array(bool shape[16][16]) {
  int i = 0;
  for (int j = 0; j < 16; j++) {
    unsigned int value = 0;
    unsigned int temp = 1;
    for (i = 0; i < 16; i++) {
      if (i != 0)
        temp = temp * 2;
      value = value + int(shape[i][j]) * temp;
    }
    shift_resistor_receive_data(a[j], value);       //constantly column is moving right and column wise data of the row is getting printed
    delayMicroseconds(100);                         //a[j] value is for one column, and value data from the 2d array representing a single LED
    shift_resistor_receive_data(0x0000, 0xFFFF);    //clears whole matrix before the next data is displayed on the screen
  }
}


// Can be used for left shifting of 16x16 matrix
// Don't edit this function, working properly
void shift_array(bool shape[16][16]) {
  int i = 0;
  int j;

  for (i = 0; i < 16; i++) {

    bool temp = shape[i][0];
    for (j = 0; j < 16; j++) {
      shape[i][j] = shape[i][j + 1];
    }
    shape[i][15] = temp;
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(DS_pin1, OUTPUT);
  pinMode(SH_CP_pin1, OUTPUT);
  pinMode(ST_CP_pin1, OUTPUT);
  pinMode(DS_pin2, OUTPUT);
  pinMode(SH_CP_pin2, OUTPUT);
  pinMode(ST_CP_pin2, OUTPUT);
  pinMode(Vx, INPUT);
  pinMode(Vy, INPUT);
  pinMode(Sw, INPUT);
  snakeHighScore = EEPROM.read(snakeHigh);
  bricksHighScore = EEPROM.read(bricksHigh);
  shift_resistor_receive_data(0xFFFF, 0x0000);      //lights all the LEDs to check which ones are defective ones
  delay(100);
}


bool mainDisplay[16][16] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},             //first game screen
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


bool game1[16][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1}
};


uint16_t LED_Matrix[16] = {
  61223 ,
  61163 ,
  60971 ,
  61163 ,
  62247 ,
  65535 ,
  23315 ,
  5557 ,
  20915 ,
  21941 ,
  21941 ,
  64351 ,
  64351 ,
  64447 ,
  64351 ,
  64351 ,
};


uint16_t menu[16] = {
  61439 ,
  53247 ,
  36863 ,
  53231 ,
  53191 ,
  53123 ,
  2047 ,
  65535 ,
  36863 ,
  2047 ,
  26499 ,
  59335 ,
  53231 ,
  34815 ,
  2047 ,
  65535 ,
};


uint16_t gameOver[16] = {
  65535 ,
  65535 ,
  39769 ,
  29975 ,
  20753 ,
  21847 ,
  38233 ,
  65535 ,
  65535 ,
  56009 ,
  43706 ,
  43657 ,
  43706 ,
  56778 ,
  65535 ,
  65535 ,
};


uint16_t homeGame1[16] = {
  0 ,
  32766 ,
  32290 ,
  32290 ,
  32290 ,
  32766 ,
  25122 ,
  25122 ,
  25122 ,
  32766 ,
  25122 ,
  25122 ,
  25122 ,
  32766 ,
  32766 ,
  0 ,
};


uint16_t sscore[16] = {
  65535 ,
  65535 ,
  65535 ,
  65535 ,
  34959 ,
  48047 ,
  35759 ,
  60335 ,
  34959 ,                                            ////for displaying score led
  65535 ,
  64583 ,
  64861 ,
  64583 ,
  64733 ,
  64839 ,
  65535 ,
};


uint16_t highscore[16] = {
  65535 ,
  43541 ,
  43765 ,
  35473 ,
  43733 ,
  43541 ,
  65535 ,
  34959 ,
  48047 ,                                         ////for displaying highscore
  35727 ,
  60415 ,
  34952 ,
  65451 ,
  65416 ,
  65435 ,
  65448 ,
};




//#######################################################################################

// GAME :: BLOCK BREAKER
// Block Breaker Game Program Starts here




// draw paddle position from analog input for the game
// Don't change this function working fine
void paddlePosition() {
  //Right = 0
  //Left = 1023
  int i = 0;
  int sensorValue = analogRead(A0);

  if (sensorValue > 852)  // Extreme Left
    for (i = 0; i < 16; i++)
    {
      if (i >= 0 && i < 5)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }

  else if (sensorValue > 652 && sensorValue < 853)
    for (i = 0; i < 16; i++)
    {
      if (i > 1 && i < 7)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }

  else if (sensorValue > 512)
    for (i = 0; i < 16; i++) {
      if (i > 3 && i < 9)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }

  else if (sensorValue < 512 && sensorValue > 342)
    for (i = 0; i < 16; i++) {
      if (i > 5 && i < 11)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }

  else if (sensorValue < 342 && sensorValue > 172)
    for (i = 0; i < 16; i++) {
      if (i > 8 && i < 14)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }

  else if (sensorValue < 172) // Extreme Right
    for (i = 0; i < 16; i++) {
      if (i > 10 && i <= 15)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }

  else
    for (i = 0; i < 16; i++) {
      if (i > 5 && i < 11)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }
}


// move ball
void moveBall() {
  game1[y][x] = 1;
  // check wall collisions and reverse

  x -= right;
  y -= up;




  if (game1[y][x] == 0)
  {
    up = - up;
  }
  else if (game1[y][x] == 0 && game1[y + 1][x] == 0 && game1[y][x - 1] == 0)
  {
    game1[y + 1][x] = 1;
    game1[y][x - 1] = 1;
    up = - up;
  }
  if (game1[15][x] == 0 && y == 14)
  {
    up = - up;
  }

  if (x == 0 || x == 15)
    right = -right;
  if (y == 0 )
    up = -up;
  game1[y][x] = 0;
}


//calculates score at the end of game
int scoreBlock() {
  int score = 0;
  for(int i=0; i<4; i++){
    for(int j=0; j<16; j++){
      if(game1[i][j] == 1)
        score++;
    }
  }

  return score;
}


void startGame1() {
  
  int i = 0;
  bool endGame1 = false;

  for (i = 0; i < 80; i++) {
    hexToArray(homeGame1, mainDisplay);
    print_array(mainDisplay);
  }


  game1[y][x] = 0;                //starting position of ball

  while (!endGame1) {
    for (i = 0; i < 15; i++) {
      print_array(game1);
      paddlePosition();
    }
    moveBall();
    if (y == 15)
      endGame1 = true;
  }
    score = scoreBlock() ;
    if(score > bricksHighScore) {
    bricksHighScore = score;                                    
    EEPROM.write(bricksHigh, bricksHighScore);                     //////to save highscore in eeprom memory 
    }
    clearScreen(mainDisplay);
    hexToArray(sscore,mainDisplay);
    for(uint8_t i=0; i<100; i++){
     print_array(mainDisplay);
     delay(10);
     }
     printtwodigitnumber(score) ;
      delay(100) ;
    clearScreen(mainDisplay);
    hexToArray(highscore,mainDisplay);
    for(uint8_t i=0; i<100; i++){
       print_array(mainDisplay);
       delay(10);
     }
     printtwodigitnumber(bricksHighScore) ;
     delay(100) ;
    clearScreen(mainDisplay);
  for (i = 0; i < 50; i++) {
     hexToArray(gameOver, mainDisplay);
     print_array(mainDisplay);
  }
}

// Block Breaker Game Program Finishes here

//#######################################################################################





//#######################################################################################
// GAME :: SNAKE GAME
// Snake Game Program Starts here
void clearScreen(bool x[][16]) {
  for (uint8_t i = 0; i < 16; i++) {
    for (uint8_t j = 0; j < 16; j++) {
      x[i][j] = 1;
    }
  }
}


uint16_t snakeGameStartScreen[16] = {
  0,
  32766,
  17066,
  24102,
  24234,
  17406,
  31586,
  31278,
  17058,
  32750,
  32738,
  32750,
  31502,
  32766,
  32766,
  0
};


void snakeSetup() {
  spawnApple();
  //  Serial.begin(9600);
  //  startmessage();

}


bool bigAppleEaten = false;


void spawnApple() {
  bool k = true;
  while (k) {
    k = false;
    applePos.x = random(1, 16); //spawns apple and random positions
    applePos.y = random(1, 16);
    for (uint8_t i = 0; i < snakeLength; i++) {
      if (positions[i].y == applePos.y && positions[i].x == applePos.x) {
        k = true;
      }
    }
  }
}


void drawSnake() {
  for (uint8_t i = 0; i < snakeLength; i++) {
    mainDisplay[positions[i].y][positions[i].x] = 0;

  }
  if (bigAppleEaten) {
    mainDisplay[applePos.y][applePos.x] = 0;
  }
  else {
    for (uint8_t i = applePos.y - 1; i < applePos.y + 2; i++) {
      for (uint8_t j = applePos.x - 1; j < applePos.x + 2; j++) {
        mainDisplay[i][j] = 0;
      }
    }
  }
  print_array(mainDisplay);
}


void snakeGameOver() {
  if (score > snakeHighScore) {
    snakeHighScore = score;
    EEPROM.write(snakeHigh, snakeHighScore);                     //////to save highscore in eeprom memory
  }
  clearScreen(mainDisplay);
  hexToArray(sscore, mainDisplay);
  for (uint8_t i = 0; i < 100; i++) {
    print_array(mainDisplay);
    delay(10);
  }
  printtwodigitnumber(score) ;
  delay(100) ;
  clearScreen(mainDisplay);
  hexToArray(highscore, mainDisplay);
  for (uint8_t i = 0; i < 100; i++) {
    print_array(mainDisplay);
    delay(10);
  }
  printtwodigitnumber(snakeHighScore) ;
  delay(100) ;
  bigAppleEaten = false;
  clearScreen(mainDisplay);
  hexToArray(gameOver, mainDisplay);
  for (uint8_t i = 0; i < 100; i++) {
    print_array(mainDisplay);
    delay(10);
  }
  snakeLength = 1;
  score = 0 ;
  startGame();
}


void startSnakeGame() {
  hexToArray(snakeGameStartScreen, mainDisplay);
  for (uint8_t i = 0; i < 40; i++) {
    print_array(mainDisplay);
    delay(10);
  }
  spawnApple();
  while (1) {
    int x, y, s;
    x = analogRead(Vx);
    //x*=-1;
    y = analogRead(Vy);
    //y*=-1;
    s = analogRead(Sw);
    if (direc == DOWN || direc == UP) {
      if (x < LESS) {
        direc = RIGHT;
      }
      else if (x > MORE) {
        direc = LEFT;
      }
    }
    else if (direc == LEFT || direc == RIGHT) {
      if (y > MORE) {
        direc = UP;
      }

      else if (y < LESS) {

        direc = DOWN;
      }
    }

    if (direc == UP) {
      pos->y -= 1;
    }
    else if (direc == LEFT) {
      pos->x -= 1;
    }
    else if (direc == DOWN) {
      pos->y += 1;
    }
    else if (direc == RIGHT) {
      pos->x += 1;
    }

    if (pos->x > 15 || pos->x < 1 || pos->y < 1 || pos->y > 15) {
      if (pos->x > 15) {
        pos->x = 0;
      }
      else if (pos->x < 0) {
        pos->x = 15;
      }
      if (pos->y > 15) {
        pos->y = 0;
      }
      else if (pos->y < 0) {
        pos->y = 15;
      }

      //      scoreprint();            //prints score
      //      delay(500);
    }


    if (bigAppleEaten) {
      if (pos->y == applePos.y && pos->x == applePos.x) {
        snakeLength++;
        score += 1;
        spawnApple();
      }
    }
    else {
      if (abs(pos->y - applePos.y) <= 1 && abs(pos->x - applePos.x) <= 1) {
        snakeLength++;
        score += 1;
        spawnApple();
        bigAppleEaten = true;
      }
    }
    for (int i = snakeLength; i > 0; i--) {
      positions[i].x = positions[i - 1].x;
      positions[i].y = positions[i - 1].y;

    }
    if (snakeLength > 4) {
      for (int i = 5; i < snakeLength; i++) {
        if (pos->x == positions[i].x && pos->y == positions[i].y) {
          ///GAME OVER
          snakeGameOver();


        }
      }
    }

    for (int i = 0; i < 10; i++) {
      drawSnake();
      delay(10);
    }
    clearScreen(mainDisplay);

  }
}


// Snake Game Game Program Finishes here

//#######################################################################################





// Menu asking user which game to play
void startGame() {
  int i = 0;
  for (i = 0; i < 80; i++) {
    hexToArray(LED_Matrix, mainDisplay);
    print_array(mainDisplay);
  }

  int option = analogRead(Vy);

  // Serial.println(option);
  //  option=500;
  while (option > 400 && option < 600) {
    option = analogRead(Vy);
    hexToArray(menu, mainDisplay);
    print_array(mainDisplay);
  }

  if (option < 400)
    startGame1();
  if (option > 600)
    startSnakeGame();
}



void loop() {
  startGame();
}
