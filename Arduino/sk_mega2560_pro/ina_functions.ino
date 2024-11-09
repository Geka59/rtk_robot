//#include <Wire.h>
#include <Beastdevices_INA3221.h>
unsigned long ina_timer;
#define SERIAL_SPEED      115200    // serial baud rate
#define PRINT_DEC_POINTS  3         // decimal points to print

// Set I2C address to 0x41 (A0 pin -> VCC)
Beastdevices_INA3221 ina3221(INA3221_ADDR41_VCC);

void ina_setup() {
  Serial.begin(SERIAL_SPEED);

  while (!Serial) {
      delay(1);
  }

  ina3221.begin();
  ina3221.reset();

  // Set shunt resistors to 10 mOhm for all channels
  ina3221.setShuntRes(10, 10, 10);
}

void ina_loop() {
  if ((millis()-ina_timer)>100){
  float current[3];
  float voltage[3];

  current[0] = ina3221.getCurrent(INA3221_CH1);
  voltage[0] = ina3221.getVoltage(INA3221_CH1);

  current[1] = ina3221.getCurrent(INA3221_CH2);
  voltage[1] = ina3221.getVoltage(INA3221_CH2);

  current[2] = ina3221.getCurrent(INA3221_CH3);
  voltage[2] = ina3221.getVoltage(INA3221_CH3);

  Serial.print("Channel 1: ");
  Serial.print(current[0], PRINT_DEC_POINTS);
  Serial.print("A, ");
  Serial.print(voltage[0], PRINT_DEC_POINTS);
  Serial.println("V");

  Serial.print("Channel 2: ");
  Serial.print(current[1], PRINT_DEC_POINTS);
  Serial.print("A, ");
  Serial.print(voltage[1], PRINT_DEC_POINTS);
  Serial.println("V");

  Serial.print("Channel 3: ");
  Serial.print(current[2], PRINT_DEC_POINTS);
  Serial.print("A, ");
  Serial.print(voltage[2], PRINT_DEC_POINTS);
  Serial.println("V");
  ina_timer=millis();
  
  
  }

  
}





////0x41 второй датчик база 040
//#include "SDL_Arduino_INA3221.h"
//SDL_Arduino_INA3221 ina3221{0x41,0.1f};
////SDL_Arduino_INA3221 ina3221_1{0x40,0.1f};
//unsigned long ina_timer;
//void ina_setup(){
//  ina3221.begin();
//  //ina3221_1.begin();
//
//  Serial.print("Manufactures ID=0x");
//  int MID;
//  MID = ina3221.getManufID();
//  Serial.println(MID,HEX);
//
////   Serial.print("Manufactures ID=0x");
////  int MID1;
////  MID1 = ina3221_1.getManufID();
////  Serial.println(MID1,HEX);
//  }
//
//
// static float current_mA1l = 0;
// static float current_mA2l = 0;
// static float current_mA3l = 0;
//void ina_loop(){
//   static float current_mA1 = 0;
// static float current_mA2= 0;
// static float current_mA3 = 0;
//  if ((millis()-ina_timer)>70){
//  
//  current_mA1 = ina3221.getCurrent_mA(1);
//  current_mA2 = ina3221.getCurrent_mA(2);
//  current_mA3 = ina3221.getCurrent_mA(3);
//  
////  current_mA1l = ina3221_1.getCurrent_mA(1);
////  current_mA2l = ina3221_1.getCurrent_mA(2);
////  current_mA3l = ina3221_1.getCurrent_mA(3);
//  Serial.println(current_mA1);
//  //current_mA3 = ina3221.getCurrent_mA(OUTPUT_CHANNEL);
//  ina_timer=millis();}
//  
//  }
