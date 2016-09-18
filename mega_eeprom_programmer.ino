//***************************************************************************************************************************************************************
//********************************************************* CHANGE THESE VALUES !!!******************************************************************************
//***************************************************************************************************************************************************************

//HARD CODED hex values to be written to rom. Change this to your specifications
byte program_dat[] = {
  0x3E, 0x00, 0x3E, 0x03, 0xD3, 0x00, 0x3E, 0x15, 0xD3, 0x00, 0x06, 0x41, 0x3E, 0x00, 0xDB, 0x00, 0xCB, 0x4F, 0xCA, 0x0C, 0x00, 0x78, 0xD3, 0x01, 0x78, 0xFE, 
  0x5B, 0xCA, 0x22, 0x00, 0x04, 0xC3, 0x0C, 0x00, 0x76
};

int led = 13; //led for debugging

//EEPROM control signals, tie to VDD with pullup resistor. Change to accomodate your setup
int oe = 9;
int cs = 10;
int we = 11;

//data & address lines of EEPROM. Change to accomodate your setup
int dbit0 = A8;
int dbit1 = A9;
int dbit2 = A10;
int dbit3 = A11;
int dbit4 = A12;
int dbit5 = A13;
int dbit6 = A14;
int dbit7 = A15;

int dbArray [] = {dbit0, dbit1, dbit2, dbit3, dbit4, dbit5, dbit6, dbit7}; 


//pin definitions for address lines. Add/Remove to accomodate your particular chip. DONT FORGET TO ADD/REMOVE ADDITIONAL PIN DEFINITIONS TO abArray.
int abit0 = A7;
int abit1 = A6;
int abit2 = A5;
int abit3 = A4;
int abit4 = A3;
int abit5 = A2;
int abit6 = A1;
int abit7 = A0;
int abit8 = 34;
int abit9 = 35;
int abit10 = 36;
int abit11 = 37;
int abit12 = 38;
//....

//ADDITIONAL ADDRESS PIN DEFINITIONS GO HERE!!!
int abArray [] = {abit0, abit1, abit2, abit3, abit4, abit5, abit6, abit7, abit8, abit9, abit10, abit11, abit12}; 


//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************
//***************************************************************************************************************************************************************

//array that holds a byte as individual bits, used for reading from rom
int read_data [] = {0,0,0,0,0,0,0,0};

void setup() {               
  Serial.begin(9600);

  //set pins connected to eeprom control signals as outputs, set them HIGH
  pinMode(we, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(oe, OUTPUT);
  digitalWrite(cs, HIGH);
  digitalWrite(we, HIGH);
  digitalWrite(oe, HIGH);

  for (int i = 0; i < sizeof(abArray)/sizeof(int); i++){
    pinMode(abArray[i], OUTPUT);
  }

  for (int i = 0; i < sizeof(dbArray)/sizeof(int); i++){
    pinMode(dbArray[i], OUTPUT);
  }

}

void loop() {
  //iterate through byte array holding data to be written
  for (int i = 0; i < sizeof(program_dat)/sizeof(byte); i++){
    digitalWrite(cs, HIGH); //make sure chip isn't enabled
    addrWrite(i); //write current position of array to address lines
    dataWrite(program_dat[i]); //write data at current position of array to data lines
    digitalWrite(cs, HIGH); //disable chip
    digitalWrite(we, HIGH); //disable chip write enable
  }
  
  digitalWrite(led,HIGH);
 
  digitalWrite(cs, HIGH); //disable chip
  digitalWrite(we, HIGH); //disable chip write enable

//************************************

  //set chip we to read, and oe to output mode
  digitalWrite(we,HIGH);
  digitalWrite(oe,LOW);

  //read back rom contents and print them out
  readRom();

  //infinite loop to avoid going back to beginning
  while(1){}

}

//function that writes int value address to chip address lines
void addrWrite(int address){
  digitalWrite(cs, HIGH);
  digitalWrite(we, HIGH);
  int addr = address;

  //convert address into binary value, write value to address lines
  for (int i = 0; i < sizeof(abArray)/sizeof(int); i++){
    if((addr & 1) == 1){
      digitalWrite(abArray[i], HIGH);
    }
    else{
      digitalWrite(abArray[i], LOW);
    }
    addr = addr >> 1;
  }

}

void dataWrite(int data){
  //make sure chip is disabled, as well as write
  digitalWrite(we, HIGH);
  digitalWrite(cs, HIGH);
  
  int dat = data;

  //convert data into binary value, write value to data lines
  for(int i = 0; i<=7; i++){
   
    if((dat & 1) == 1){
      digitalWrite(dbArray[i], HIGH);
    }
    else{
      digitalWrite(dbArray[i], LOW);
    }
    dat = dat >> 1;

   
  }
  delay(5); //delay so things settle
  //enable chip, and latch values in data lines onto chip
  digitalWrite(cs, LOW); 
  digitalWrite(we, LOW);
}

//function that reads a value from ROM wherever address lines are pointing to
void dataRead(){
  digitalWrite(cs, LOW); //.......................
  digitalWrite(we, LOW); //.......................

  //read binary data value at address lines, store binary stream in an array
  for(int i = 0; i<=7; i++){
   
    if((digitalRead(dbArray[i])) == HIGH){
      read_data[i]=1;
    }
    else{
      read_data[i]=0;
    }
   
  }
  delay(2);

  //perform bit shifting magic to convert acquired binary stream into an int
  int readValue = 0x00;
  int myPow = 1;
  for(int i = 0; i < 8; i++){
    readValue += (read_data[i]*myPow);
    myPow = myPow*2;
  }

  //print out converted value in HEX format
  Serial.println(readValue, HEX);
  Serial.println();

  //disable chip, disable write
  digitalWrite(cs, HIGH); //.......................
  digitalWrite(we, HIGH);

}

//function that simply reads out contents of rom for visual verification
void readRom(){
  //make sure chip and read disabled
  digitalWrite(cs, HIGH);
  digitalWrite(we, HIGH);

  //address pins remain outputs, but data pins are now inputs. DONT FORGET TO CONFIGURE ANY NEWLY ADDED ADDRESS PINS HERE
  for (int i = 0; i < sizeof(abArray)/sizeof(int); i++){
    pinMode(abArray[i], OUTPUT);
  }

  for (int i = 0; i < sizeof(dbArray)/sizeof(int); i++){
    pinMode(dbArray[i], INPUT);
  }

  digitalWrite(we, HIGH); 
  digitalWrite(cs, LOW);

    Serial.println("************************");
    delay(100);
      for (int i = 0; i < sizeof(program_dat)/sizeof(byte); i++){
        addrWrite(i);
        dataRead();
      }
    Serial.println("************************");
}


