#define DS_pin1  7            //for columns (anode)
#define SH_CP_pin1  5
#define ST_CP_pin1  6

#define DS_pin2  4            //for rows (cathode)
#define SH_CP_pin2  13
#define ST_CP_pin2  3

unsigned int a[16] = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
//values for a single column at a time, starting from the leftmost column to the rightmost column

void shift_resistor_receive_data(int data1, int data2){
  for (int i = 0; i < 16; i++){
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

  for (int i = 0; i < 16; i++){
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

void print_array(bool shape[16][16]){
  for (int j = 0; j < 16; j++){
    unsigned int value = 0;
    unsigned int temp = 1;
    for (int i = 0; i < 16; i++){
      if (i != 0)
        temp = temp * 2;
      value = value + int(shape[i][j]) * temp;
    }
    shift_resistor_receive_data(a[j], value);       //constantly column is moving right and column wise data of the row is getting printed
    delayMicroseconds(100);                         //a[j] value is for one column, and value data from the 2d array representing a single LED
    shift_resistor_receive_data(0x0000, 0xFFFF);    //clears whole matrix before the next data is displayed on the screen
  }
}

void scroll_array(bool shape[16][16]){
  for(int j=0; j<16; j++){
    int value =0, temp=0;
    for (int i = 0; i < 16; i++){
      if (i != 0)
        temp = temp * 2;
      value = value + int(shape[i][j]) * temp;
    }
    shift_resistor_receive_data(a[j], value);       //constantly column is moving right and column wise data of the row is getting printed
    delayMicroseconds(100);                         //a[j] value is for one column, and value data from the 2d array representing a single LED
    shift_resistor_receive_data(0x0000, 0xFFFF);    //clears whole matrix before the next data is displayed on the screen
  }
}

void LED_On(int ST_CP, int DS, int SH_CP, int value1, int value2){
  digitalWrite(ST_CP, LOW);
  shiftOut(DS, SH_CP, MSBFIRST, value1);
  shiftOut(DS, SH_CP, MSBFIRST, value2);
  digitalWrite(ST_CP, HIGH);
}

void shift_array(bool shape[16][16]){
  int i, j,temp;
  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      if(j==0){
        shape[i][15]=shape[i][0];
      }
      
        if(shape[i][j] == 0){
          temp = shape[i][j-1];
          shape[i][j-1] = shape[i][j];
          shape[i][j] = temp;
      
      }
    }
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

  shift_resistor_receive_data(0xFFFF, 0x0000);      //lights all the LEDs to check which ones are defective ones
  delay(100);
}

bool game[16][16]={
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
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

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

bool menu[16][16]={
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void loop(){
  int k=0;
  
  for(k=0; k<100; k++){
    print_array(LED_Matrix);
  }
  
  for(k=0; k<100; k++){
    print_array(menu);
  }
  //shift_array(LED_Matrix);
}
