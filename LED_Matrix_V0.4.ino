#define DS_pin1  7            //for columns (anode)
#define SH_CP_pin1  5
#define ST_CP_pin1  6

#define DS_pin2  4            //for rows (cathode)
#define SH_CP_pin2  13
#define ST_CP_pin2  3

#define Vx A0                 //Joystick
#define Vy A1
#define Sw A2


// Global Constant



// Global constants for BLOCK BREAKER
int x=8, y=14, right=1, up=1;


unsigned int a[16] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
//values for a single column at a time, starting from the leftmost column to the rightmost column


// Don't edit this function, working properly
void shift_resistor_receive_data(int data1, int data2){
  int i=0;
  for (i = 0; i < 16; i++){
    if ( data1 & 0b00000000000000001){
      digitalWrite(DS_pin1, HIGH);
    }
    else{
      digitalWrite(DS_pin1, LOW);
    }
    data1 = data1 >> 1;
    digitalWrite(SH_CP_pin1, HIGH);
    digitalWrite(SH_CP_pin1, LOW);
  }
  digitalWrite(ST_CP_pin1, HIGH);
  digitalWrite(ST_CP_pin1, LOW);

  for (i = 0; i < 16; i++){
    if ( data2 & 0b1000000000000000){
      digitalWrite(DS_pin2, HIGH);
    }
    else{
      digitalWrite(DS_pin2, LOW);
    }
    data2 = data2 << 1;
    digitalWrite(SH_CP_pin2, HIGH);
    digitalWrite(SH_CP_pin2, LOW);
  }
  digitalWrite(ST_CP_pin2, HIGH);
  digitalWrite(ST_CP_pin2, LOW);
}


// Don't edit this function, working properly
void print_array(bool shape[16][16]){
  int i=0;
  for (int j = 0; j < 16; j++){
    unsigned int value = 0;
    unsigned int temp = 1;
    for (i = 0; i < 16; i++){
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
void shift_array(bool shape[16][16]){
  int i=0;
  int j;
  
  for(i=0; i<16; i++){
    
    bool temp=shape[i][0];
    for(j=0; j<16; j++){
      shape[i][j]=shape[i][j+1];
    }
    
    shape[i][15]=temp;
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

  shift_resistor_receive_data(0xFFFF, 0x0000);      //lights all the LEDs to check which ones are defective ones
  delay(100);
}


bool LED_Matrix[16][16]={
  {1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
  {1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1},
  {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
  {1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1},
  {0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1},
  {0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1},
  {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1},
  {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1},
  {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1}
};


bool menu[16][16] = {
  {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


bool push[16][16]={
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1},
  {1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1}
};


// holds the screen till the user pushes the center button
// Don't edit this function, working properly
void waitForPush(){
  int s = 100, count = 5, switchValue=0;
  
  while(count--){
    s = analogRead(Sw);
    switchValue += s;
  }
  switchValue /= 5;

  while(switchValue){
    count = 5;
    print_array(push);
    
    while(count--){
      s = analogRead(Sw);
      switchValue += s;
    }
    switchValue /= 5;
  }
}


bool gameOver[16][16]={
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1},
  {0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1},
  {0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
  {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1},
  {1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
  {1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};



//#######################################################################################

// GAME :: BLOCK BREAKER
// Block Breaker Game Program Starts here

bool homeGame1[16][16]={
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


bool game1[16][16]={
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


// draw paddle position from analog input for the game
// Don't change this function working fine
void paddlePosition(){  
  //Right = 0
  //Left = 1023
  int i=0;
  int sensorValue = analogRead(A0);
  
  if (sensorValue > 852)  // Extreme Left
    for(i=0; i<16; i++){
      if(i>=0 && i < 5)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }
    
  else if (sensorValue > 652 && sensorValue < 853)
    for(i=0; i<16; i++){
      if(i>1 && i < 7)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }
    
  else if (sensorValue > 512)
    for(i=0; i<16; i++){
    if(i>3 && i < 9)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }
    
  else if (sensorValue < 512 && sensorValue > 342)
    for(i=0; i<16; i++){
    if(i>5 && i < 11)
        game1[15][i] = 0;
    else
        game1[15][i] = 1;
    }
    
  else if (sensorValue < 342 && sensorValue > 172)
    for(i=0; i<16; i++){
    if(i>8 && i < 14)
        game1[15][i] = 0;
    else
        game1[15][i] = 1;
    }
    
  else if (sensorValue < 172) // Extreme Right
    for(i=0; i<16; i++){
    if(i>10 && i <= 15)
        game1[15][i] = 0;
    else
        game1[15][i] = 1;
    }
    
  else
    for(i=0; i<16; i++){
      if(i>5 && i < 11)
        game1[15][i] = 0;
      else
        game1[15][i] = 1;
    }
}


// move ball
void moveBall(){
  game1[y][x]=1;
  // check wall collisions and reverse

    x -= right;
    y -= up;
        
 
  
  
  if(game1[y][x]==0)
  {
   up =- up;
  }
  else if(game1[y][x]==0 && game1[y+1][x]==0 && game1[y][x-1]==0)
  {
   game1[y+1][x]=1;
   game1[y][x-1]=1;
   up =- up; 
  }
  if(game1[15][x]==0 && y == 14)
  {
    up =- up;
  }
  
  if(x == 0 || x == 15)
    right = -right;
  if(y == 0 )
    up = -up;
    
 
   
game1[y][x]=0;
//  // check block or paddle collisions, delete block and reverse
//  if(game1[y+up][x] == 0){
//    game1[15-x][15-y-up] = 1;
//    up = -up;
//  }
//  if (game1[y][x+right] == 0) {
//    game1[15-x-right][15-y] = 1;
//    right = -right;
//  }
//  if (game1[y+up][x+right] == 0) {
//    game1[15-x-right][15-y-up] = 1;
//    right = -right;
//    up = -up;
//  
//    // more potential for collisions after diagnal bounce so check again
//    if (x == 0 || x == 15) {
//      right = -right;
//    }
//    if (y == 0 || y == 15) {
//      up = -up;
//    }
//    if (game1[y+up][x+right] == 0) {
//      game1[15-x-right][15-y-up] = 1;
//      up = -up;
//      right = -right;
//    }
//
//
//  
//  }

  // set new posistion
//  y += up;
//  x += right;
//
//  
//  // draw new ball posistion
//  game1[y][x] = 0;

  
}


void startGame1(){
  int i=0;
  bool endGame1 = false;
  
  for(i=0; i<80; i++){
    print_array(homeGame1);
  }

  //waitForPush();

  game1[y][x] = 0;                //starting position of ball
  
  while(!endGame1){
    for(i=0; i<15; i++){
      print_array(game1);
      paddlePosition();
    }
    moveBall();
    if(y==15)
    endGame1 = true;
  }

  for(i=0; i<50; i++){
    print_array(gameOver);
  }
}

// Block Breaker Game Program Finishes here

//#######################################################################################





//#######################################################################################
// GAME :: SNAKE GAME
// Snake Game Program Starts here



void startGame2(){
  
}

// Snake Game Game Program Finishes here

//#######################################################################################





// Menu asking user which game to play
void startGame(){
  int i=0;
  
  for(i=0; i<80; i++){
    print_array(LED_Matrix);
  }
  
  int option = analogRead(Vy);
  
  Serial.println(option);
  while(option > 400 && option < 600){
    option = analogRead(Vy);
    print_array(menu);
    Serial.println(option);
  }

  if(option < 400)
    startGame1();
  if(option > 600)
    startGame2();
}



void loop() {
  print_array(push);
  startGame();
}
