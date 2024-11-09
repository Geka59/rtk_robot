#include <MPU9250_WE.h>
#include <Wire.h>
#define MPU9250_ADDR 0x68
unsigned long timer_mpu;

MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);
void Mpu_setup() {
  Wire.begin();
  Wire.setClock(400000);
  if(!myMPU9250.init()){
    Serial.println("MPU9250 does not respond");
  }
  else{
    Serial.println("MPU9250 is connected");
  }
 
  /* The slope of the curve of acceleration vs measured values fits quite well to the theoretical 
   * values, e.g. 16384 units/g in the +/- 2g range. But the starting point, if you position the 
   * MPU9250 flat, is not necessarily 0g/0g/1g for x/y/z. The autoOffset function measures offset 
   * values. It assumes your MPU9250 is positioned flat with its x,y-plane. The more you deviate 
   * from this, the less accurate will be your results.
   * The function also measures the offset of the gyroscope data. The gyroscope offset does not   
   * depend on the positioning.
   * This function needs to be called at the beginning since it can overwrite your settings!
   */
  Serial.println("Position you MPU9250 flat and don't move it - calibrating...");
  delay(1000);
  myMPU9250.autoOffsets();
  Serial.println("Done!");

  
  //myMPU9250.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);
  
  myMPU9250.setSampleRateDivider(5);

  /*  MPU9250_ACC_RANGE_2G      2 g    (default)
   *  MPU9250_ACC_RANGE_4G      4 g
   *  MPU9250_ACC_RANGE_8G      8 g   
   *  MPU9250_ACC_RANGE_16G    16 g
   */
  myMPU9250.setAccRange(MPU9250_ACC_RANGE_2G);

  /*  Enable/disable the digital low pass filter for the accelerometer 
   *  If disabled the bandwidth is 1.13 kHz, delay is 0.75 ms, output rate is 4 kHz
   */
  myMPU9250.enableAccDLPF(true);


  /*  Digital low pass filter (DLPF) for the accelerometer, if enabled 
   *  MPU9250_DPLF_0, MPU9250_DPLF_2, ...... MPU9250_DPLF_7 
   *   DLPF     Bandwidth [Hz]      Delay [ms]    Output rate [kHz]
   *     0           460               1.94           1
   *     1           184               5.80           1
   *     2            92               7.80           1
   *     3            41              11.80           1
   *     4            20              19.80           1
   *     5            10              35.70           1
   *     6             5              66.96           1
   *     7           460               1.94           1
   */
  myMPU9250.setAccDLPF(MPU9250_DLPF_6);  
}

void Mpu_loop() {
  if (millis()-timer_mpu>70){
  xyzFloat angles = myMPU9250.getAngles();
  

  //Serial.print("Angle x = ");
  //Serial.print(angles.x);
  //Serial.print("  |  Angle y = ");
  //Serial.print(angles.y);
  //Serial.print("  |  Angle z = ");
  //Serial.println(angles.z);

///* Pitch and roll consider all axes for calculation. According to my experience
   //it provides more reliable results at higher angles (>60°) */
  float pitch = myMPU9250.getPitch();
  float roll  = myMPU9250.getRoll();
  mpu_roll = myMPU9250.getRoll();
  //Serial.print("Pitch   = "); 
  //Serial.print(pitch); 
  //Serial.print("  |  Roll    = "); /// на автономный подъем
  //Serial.println(roll); 
  
  //Serial.println();
  timer_mpu=millis();}
  
  
}














































//#include "Wire.h"
//#include "I2C.h"
// 
//#define MPU9250_IMU_ADDRESS 0x68
//#define MPU9250_MAG_ADDRESS 0x0C
// 
//#define GYRO_FULL_SCALE_250_DPS  0x00
//#define GYRO_FULL_SCALE_500_DPS  0x08
//#define GYRO_FULL_SCALE_1000_DPS 0x10
//#define GYRO_FULL_SCALE_2000_DPS 0x18
// 
//#define ACC_FULL_SCALE_2G  0x00
//#define ACC_FULL_SCALE_4G  0x08
//#define ACC_FULL_SCALE_8G  0x10
//#define ACC_FULL_SCALE_16G 0x18
// 
//#define TEMPERATURE_OFFSET 21 // As defined in documentation
// 
//#define INTERVAL_MS_PRINT 1000
// 
//#define G 9.80665
// 
//struct gyroscope_raw {
// int16_t x, y, z;
//} gyroscope;
// 
//struct accelerometer_raw {
//  int16_t x, y, z;
//} accelerometer;
// 
//struct magnetometer_raw {
//  int16_t x, y, z;
// 
//  struct {
// int8_t x, y, z;
// } adjustment;
//} magnetometer;
// 
//struct temperature_raw {
// int16_t value;
//} temperature;
// 
//struct {
//  struct {
//    float x, y, z;
// } accelerometer, gyroscope, magnetometer;
// 
//  float temperature;
//} normalized;
// 
//unsigned long lastPrintMillis = 0;
// 
//void setup()
//{
//  Wire.begin();
//  Serial.begin(115200);
// 
//  I2CwriteByte(MPU9250_IMU_ADDRESS, 27, GYRO_FULL_SCALE_1000_DPS); // Configure gyroscope range
//  I2CwriteByte(MPU9250_IMU_ADDRESS, 28, ACC_FULL_SCALE_2G); // Configure accelerometer range
// 
//  I2CwriteByte(MPU9250_IMU_ADDRESS, 55, 0x02); // Set by pass mode for the magnetometers
//  I2CwriteByte(MPU9250_IMU_ADDRESS, 56, 0x01); // Enable interrupt pin for raw data
// 
//  setMagnetometerAdjustmentValues();
// 
//  //Start magnetometer
//  I2CwriteByte(MPU9250_MAG_ADDRESS, 0x0A, 0x12); // Request continuous magnetometer measurements in 16 bits (mode 1)
//}
// 
//void loop()
//{
//  unsigned long currentMillis = millis();
// 
//  if (isImuReady()) {
//    readRawImu();
// 
//    normalize(gyroscope);
//    normalize(accelerometer);
//    normalize(temperature);
//  }
// 
//  if (isMagnetometerReady()) {
//    readRawMagnetometer();
// 
//    normalize(magnetometer);
//  }
// 
//  if (currentMillis - lastPrintMillis > INTERVAL_MS_PRINT) {
//    Serial.print("TEMP:\t");
//    Serial.print(normalized.temperature, 2);
//    Serial.print("\xC2\xB0"); //Print degree symbol
//    Serial.print("C");
//    Serial.println();
// 
//    Serial.print("GYR (");
//    Serial.print("\xC2\xB0"); //Print degree symbol
//    Serial.print("/s):\t");
//    Serial.print(normalized.gyroscope.x, 3);
//    Serial.print("\t\t");
//    Serial.print(normalized.gyroscope.y, 3);
//    Serial.print("\t\t");
//    Serial.print(normalized.gyroscope.z, 3);
//    Serial.println();
// 
//    Serial.print("ACC (m/s^2):\t");
//    Serial.print(normalized.accelerometer.x, 3);
//    Serial.print("\t\t");
//    Serial.print(normalized.accelerometer.y, 3);
//    Serial.print("\t\t");
//    Serial.print(normalized.accelerometer.z, 3);
//    Serial.println();
// 
//    Serial.print("MAG (");
//    Serial.print("\xce\xbc"); //Print micro symbol
//    Serial.print("T):\t");
//    Serial.print(normalized.magnetometer.x, 3);
//    Serial.print("\t\t");
//    Serial.print(normalized.magnetometer.y, 3);
//    Serial.print("\t\t");
//    Serial.print(normalized.magnetometer.z, 3);
//    Serial.println();
// 
//    Serial.println();
