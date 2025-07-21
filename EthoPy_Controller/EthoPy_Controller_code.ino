#include <EEPROM.h>

int mx_center;
int mx_lick_1;
int mx_lick_2;
bool low_center = false;
bool low_lick_1 = false;
bool low_lick_2 = false;
int CenterInput;
int LickInput1;
int LickInput2;
int v;
int LickPin1 = 8;  // output Port 1
int LickPin2 = 9;  // output Port 2
int CenterPin = 10;  // output Center Port 
int LiquidPin1 = 4;  // input Port 1
int LiquidPin2 = 5;  // input Port 2
int interruptPin = 3; // interrupt pin

void setup() {
  // Get stored thresholds
  mx_lick_1 = readIntFromEEPROM(0);
  mx_lick_2 = readIntFromEEPROM(2);
  mx_center = readIntFromEEPROM(4);

  // Setup inpup/output pins
  pinMode(interruptPin, INPUT_PULLUP);  // sets interrupt pin
  pinMode(LickPin1, OUTPUT);  // sets the pin as output Port 1
  pinMode(LickPin2, OUTPUT);  // sets the pin as output Port 2
  pinMode(CenterPin, OUTPUT);  // sets the pin as output Port Center
  pinMode(LiquidPin1, INPUT);  // sets the pin as input Port 1
  pinMode(LiquidPin2, INPUT);  // sets the pin as input Port 2
}

void loop() {
  // if interrupt, calibrate
  if (digitalRead(interruptPin) == LOW) {
    calibrate();
  }

  // Read analog values
  CenterInput = analogRead(A0);
  LickInput1 = analogRead(A2);
  LickInput2 = analogRead(A1);

  // Compare to stored thresholds
  low_lick_1 = LickInput1 > mx_lick_1;
  low_lick_2 = LickInput2 > mx_lick_2;
  low_center = CenterInput > mx_center;

  // Write digita outputs
  digitalWrite(LickPin1, low_lick_1);
  digitalWrite(LickPin2, low_lick_2);
  digitalWrite(CenterPin, low_center);
}

void calibrate() {
  // read max values to calibrate
  v =  analogRead(A2);
  writeIntIntoEEPROM(0, v);
  v =  analogRead(A1);
  writeIntIntoEEPROM(2, v);
  v =  analogRead(A0);
  writeIntIntoEEPROM(4, v);

  // Get stored thresholds
  mx_lick_1 = readIntFromEEPROM(0);
  mx_lick_2 = readIntFromEEPROM(2);
  mx_center = readIntFromEEPROM(4);
}

void writeIntIntoEEPROM(int address, int number) { 
  EEPROM.put(address, number >> 8);
  EEPROM.put(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address) {
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}
