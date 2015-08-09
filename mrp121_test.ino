#include <Servo.h>
#include "mpr121.h"
#include <Wire.h>
Servo myservo;
int irqpin = 2;
boolean touchStates[12];
int state[3]={0,0,0};
void setup() {
  pinMode(irqpin, INPUT);
  Wire.begin();
  mpr121_setup();
  Serial.begin(9600);
  myservo.attach(8);
  myservo.write(0);
  digitalWrite(irqpin, HIGH);
}

void loop() {
  if (!checkInterrupt()) {
    Wire.requestFrom(0x5A, 2);
    byte LSB = Wire.read();
    byte MSB = Wire.read();
    uint16_t touched = ((MSB << 8) | LSB);
    readTouchInputs(touched);
    process(touched);
  }
}

void readTouchInputs(uint16_t touched) {
  for (int i = 0; i < 12; i++) {
    if (touched & (1 << i)) {
      touchStates[i] = 1;
    } else {
      touchStates[i] = 0;
    }
  }
}

int process(uint16_t touched) {
  int e=0,m=0;
  int check[3] = {0, 0, 0};
  int local1 = local_1(touched);
  int local2 = local_2(touched);
  int local3 = local_3(touched);
  if (local1 > local2) {
    if (local1 > local3) {
      check[0] = 1;
    }
  }
  else if (local2 > local1) {
    if (local2 > local3) {
      check[1] = 1;
    }
  }
  else if (local3 > local1) {
    if (local3 > local2) {
      check[2] = 1;
    }
  }
  Serial.print(" ");
  for (int i = 0; i < 3; i++) {
    Serial.print(check[i]);
  }
  if (check[0] == 1) {
    state[0]=1;
    state[1]=0;
    state[2]=0;
    m=0;
  }
  if (check[1] == 1&&state[0]==1) {
    state[0]=1;
    state[1]=1;
    state[2]=0;
    m=1;
  }
  if(check[1] == 1&&state[0]!=1&&state[2]!=1){
    state[0]=0;
    state[1]=1;
    state[2]=0;
    m=0;
  }
 if(check[2] == 1&&state[1]==1){
    state[0]=0;
    state[1]=1;
    state[2]=1;
    m=1;
  }
  
 if(check[2] == 1&&state[1]!=1&&state[0]!=1){
    state[0]=0;
    state[1]=0;
    state[2]=1;
    m=0;
 }
  
  
 
  
  

  Serial.print(" ");
  for (int i = 0; i < 3; i++) {
    Serial.print(state[i]);
  }
  Serial.print(" ");
  Serial.print(m);
  Serial.println();
}

int local_1(uint16_t touched) {
  int m = 0;
  for (int i = 0; i < 4; i++) {
    if (touched & (1 << i)) {
      touchStates[i] = 1;
    } else {
      touchStates[i] = 0;
    }
    if (touchStates[i] == 1) {
      m++;
    }
  }
  return m;
}

int local_2(uint16_t touched) {
  int m = 0;
  for (int i = 4; i < 8; i++) {
    if (touched & (1 << i)) {
      touchStates[i] = 1;
    } else {
      touchStates[i] = 0;
    }
    if (touchStates[i] == 1) {
      m++;
    }
  }
  return m;
}
int local_3(uint16_t touched) {
  int m = 0;
  for (int i = 8; i < 12; i++) {
    if (touched & (1 << i)) {
      touchStates[i] = 1;
    } else {
      touchStates[i] = 0;
    }
    if (touchStates[i] == 1) {
      m++;
    }
  }
  return m;
}




void mpr121_setup(void) {

  set_register(0x5A, ELE_CFG, 0x00);

  // Section A - Controls filtering when data is > baseline
  set_register(0x5A, MHD_R, 0x01);
  set_register(0x5A, NHD_R, 0x01);
  set_register(0x5A, NCL_R, 0x00);
  set_register(0x5A, FDL_R, 0x00);

  // Section B - Controls filtering when data is < baseline.
  set_register(0x5A, MHD_F, 0x01);
  set_register(0x5A, NHD_F, 0x01);
  set_register(0x5A, NCL_F, 0xFF);
  set_register(0x5A, FDL_F, 0x02);

  // Section C - Sets touch and release thresholds for each electrode
  set_register(0x5A, ELE0_T, TOU_THRESH);
  set_register(0x5A, ELE0_R, REL_THRESH);

  set_register(0x5A, ELE1_T, TOU_THRESH);
  set_register(0x5A, ELE1_R, REL_THRESH);

  set_register(0x5A, ELE2_T, TOU_THRESH);
  set_register(0x5A, ELE2_R, REL_THRESH);

  set_register(0x5A, ELE3_T, TOU_THRESH);
  set_register(0x5A, ELE3_R, REL_THRESH);

  set_register(0x5A, ELE4_T, TOU_THRESH);
  set_register(0x5A, ELE4_R, REL_THRESH);

  set_register(0x5A, ELE5_T, TOU_THRESH);
  set_register(0x5A, ELE5_R, REL_THRESH);

  set_register(0x5A, ELE6_T, TOU_THRESH);
  set_register(0x5A, ELE6_R, REL_THRESH);

  set_register(0x5A, ELE7_T, TOU_THRESH);
  set_register(0x5A, ELE7_R, REL_THRESH);

  set_register(0x5A, ELE8_T, TOU_THRESH);
  set_register(0x5A, ELE8_R, REL_THRESH);

  set_register(0x5A, ELE9_T, TOU_THRESH);
  set_register(0x5A, ELE9_R, REL_THRESH);

  set_register(0x5A, ELE10_T, TOU_THRESH);
  set_register(0x5A, ELE10_R, REL_THRESH);

  set_register(0x5A, ELE11_T, TOU_THRESH);
  set_register(0x5A, ELE11_R, REL_THRESH);

  // Section D
  // Set the Filter Configuration
  // Set ESI2
  set_register(0x5A, FIL_CFG, 0x04);

  // Section E
  // Electrode Configuration
  // Set ELE_CFG to 0x00 to return to standby mode
  set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes


  // Section F
  // Enable Auto Config and auto Reconfig
  /*set_register(0x5A, ATO_CFG0, 0x0B);
  set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
  set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V

  set_register(0x5A, ELE_CFG, 0x0C);

}


boolean checkInterrupt(void) {
  return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v) {
  Wire.beginTransmission(address);
  Wire.write(r);
  Wire.write(v);
  Wire.endTransmission();
}
