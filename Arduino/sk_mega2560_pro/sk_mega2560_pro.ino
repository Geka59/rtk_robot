
#include <Servo.h>
///////// servo pins 9 10 11 12 flash
#define pin_s4 12
#define pin_s1_main 10 // серво манипулятор главное звено
#define pin_s2 11 // серво захват
#define pin_s3 9



//Servo servo5;
Servo servo4;
Servo servo2_catch;
Servo servo3;
//Servo servo1_l;
Servo servo1_s1_main;




#define M1_PWM 5 // ШИМ пин, на плате 6 ..2
#define M1_INA 3 // INA пин, на плате ..20
#define M1_INB 4   // INB пин, на плате  ..21

#define M2_PWM 8 // ШИМ пин, на плате ..11
#define M2_INA 6 // INA пин, на плате  ..28
#define M2_INB 7 // INB пин, на плате ..24

int xleft=127;
int yleft=127;
int xright=127;
int yright=127;
int a_button;
int b_button;
int x_button;
int dpad_left;
int dpad_right;
int dpad_up;
int dpad_down;
int y_button;
int right_shoulder;
int left_shoulder;
int r_trigger_index_pos ;
int l_trigger_index_pos;

bool flag_qr;
bool flag_b;
bool before_mode=0;
bool control_mode=0;
bool auto_mode;
int angle1=90;
//int angle1_r;
int angle2=65;
int angle3=90;
int angle4=90;
int angle5=40;

int home_angle1=15;
//int angle1_r;
int home_angle2=11;
int home_angle3=90;
int home_angle4=90;
int home_angle5=40;

//int dynamixelAngle=2047;

int qr_mode=0;
int line_mode=0;
int kin_angle3=90;


uint32_t send_timer;
uint32_t tmr1;
uint32_t atmr1;
uint32_t tmr2;
uint32_t tmr3;
uint32_t atmr4;
uint32_t atmr5;

//byte deadzone_s1_l[] = {15,180};// 0  самый перед
//byte deadzone_s1_r[] = {180,15};//180 самый перед;
////byte deadzone_s5[] = {10,79};
//byte deadzone_s2[] = {11,180};
//byte deadzone_s3[] = {0,180};
//byte deadzone_s4[] = {0,180};
//byte deadzone_s5[] = {40,130};

unsigned long auto_timer;
float mpu_roll;
bool flag_stop_up=false;

unsigned long time_stop_up;
unsigned long time_for_mpu;
bool flag_degree;

unsigned long torm_timer;
bool flag_torm=false;


bool start_parce=false;
bool good_packet=false;
int index;
unsigned char data[19];
typedef uint16_t ui16;
typedef uint8_t ui8;


ui16 crc16;
ui16 crc16_custom (ui8 *data, ui16 length1){
   ui16 crc = 0xFFFF;
    for (int i=0; i<length1; i++){
        crc ^= data[i] << 8;
        for (int j=0; j<8;j++){
            if ((crc & 0x8000) > 0)
                crc =(crc << 1) ^ 0x1021;
            else
                crc = crc << 1;}}
    return crc & 0xFFFF;}



// memcpy strunct
//unit_8t availble 
void parsing() {
  if (Serial1.available() >=22) {//>= 22
    unsigned char incomingByte = Serial1.read();
   
    if (incomingByte == 255)
    {
       for(int i = 0; i < 19; i++)
         data[i] = Serial1.read();
         crc16 = Serial1.read();
         crc16 |= ((Serial1.read() << 8) & 0xFF00);
        if (crc16_custom(data, 19) == crc16)
        {
           xleft=data[0];
           yleft=data[1];
           xright=data[2];
           yright=data[3];
            a_button=data[4];
            b_button=data[5];
            x_button=data[6];
            y_button=data[7];
            dpad_right=data[8];
            dpad_left=data[9] ;
            
            dpad_up=data[10];
            dpad_down=data[11];
            right_shoulder=data[12];
            left_shoulder=data[13];

            r_trigger_index_pos=data[14];
            l_trigger_index_pos=data[15];

            qr_mode=data[16];
            line_mode=data[17];
            kin_angle3=data[18];
        }
        else{if (Serial1.available()>0){Serial1.read();}}
       
    }
  }
 }

void send_data(){
  if (millis()-send_timer>300){Serial1.print(String(934)+","+String(344));send_timer=millis();}
  }

 int mapAngle(int angle_in){
  int microseconds=map(angle_in,0,270,520,2500);
  return microseconds;
  }
void setup() {
  delay(1000);

  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_INA, OUTPUT);
  pinMode(M1_INB, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_INA, OUTPUT);
  pinMode(M2_INB, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(57600);
  
//servo1_l.attach(pin_s1_l);
servo1_s1_main.attach(pin_s1_main);
servo2_catch.attach(pin_s2);
servo3.attach(pin_s3);
servo4.attach(pin_s4);
//  servo5.attach(pin_s5);
//  
//  servo1_l.write(180-15);
  servo1_s1_main.writeMicroseconds(mapAngle(130));
  servo2_catch.write(65);
servo3.write(90);
servo4.write(90);
//  servo5.write(44);
  Mpu_setup();
  ina_setup();



}
void loop() {
 parsing();
 //ina_loop();
 Mpu_loop();
 //send_data();


if ((control_mode!=a_button)&&(before_mode==a_button)){control_mode=a_button;}
before_mode=a_button;

//if (control_mode==0){ // УПРАВЛЕНИЕ КОЛЕСКИКАМИ
if (!auto_mode){//(r_trigger_index_pos==0&&r_trigger_index_pos==0){
  if (yright>=127){ 
  digitalWrite(M1_INA, HIGH);
  digitalWrite(M1_INB, LOW);
  analogWrite(M1_PWM, 2*(yright-127));}
  if (yright<127){ 
  digitalWrite(M1_INA, LOW);
  digitalWrite(M1_INB, HIGH);
  analogWrite(M1_PWM, 2*(127-yright));}

  ///---------------------------/M2
  if (yleft<127){ 
  digitalWrite(M2_INA, HIGH);
  digitalWrite(M2_INB, LOW);
  analogWrite(M2_PWM, 2*(127-yleft));}
  if (yleft>=127){ 
  digitalWrite(M2_INA, LOW);
  digitalWrite(M2_INB, HIGH);
  analogWrite(M2_PWM, 2*(yleft-127));}
  }

//  if(r_trigger_index_pos>0){
//  digitalWrite(M1_INA, LOW);
//  digitalWrite(M1_INB, HIGH);
//  analogWrite(M1_PWM,(r_trigger_index_pos));
//  digitalWrite(M2_INA, LOW);
//  digitalWrite(M2_INB, HIGH);
//  analogWrite(M2_PWM, (r_trigger_index_pos));
//  }
//
//  if(l_trigger_index_pos>0){
//  digitalWrite(M1_INA, HIGH);
//  digitalWrite(M1_INB, LOW);
//  analogWrite(M1_PWM,(l_trigger_index_pos));
//  digitalWrite(M2_INA, HIGH);
//  digitalWrite(M2_INB, LOW);
//  analogWrite(M2_PWM,(l_trigger_index_pos));
//  }
 
  //}// end driving ///// end of driving mode////




//if (control_mode==1){

  servo1_s1_main.writeMicroseconds(mapAngle(angle1));
  //Serial.println(angle3);
  if (angle1>220){angle2=62;}// в 60 
  servo2_catch.write((angle2));// 167 89
  servo3.write(angle3);
  servo4.write(180-angle3);
  //Serial.println(angle3);
  
  
  if ((millis()-tmr1>=10)&&(dpad_up==1)&&(angle1<237)){angle1++;tmr1=millis();} // 220 закрыть
  if ((millis()-tmr1>=10)&&(dpad_down==1)&&(angle1>0)){angle1--;tmr1=millis();} //13 на вид камеры

   if ((millis()-tmr3>=14)&&(dpad_right==1)&&(angle3<137)){angle3++;tmr3=millis();}// на качалку
   if ((millis()-tmr3>=14)&&(dpad_left==1)&&(angle3>16)){angle3--;tmr3=millis();} // на качалку 16 на line qr


  if ((millis()-tmr2>=15)&&(right_shoulder==1)&&(angle2<145)){angle2++;tmr2=millis();Serial.println("yep");}
  if ((millis()-tmr2>=15)&&(left_shoulder==1)&&(angle2>60)){angle2--;tmr2=millis();Serial.println("no yep");}



//}

if (line_mode){
  angle1=48;
  angle2=62;
  angle3=16;
  }
if (!qr_mode){flag_qr=true;}

if ((!line_mode)&&(qr_mode)&& flag_qr){// углы для чтения qr
  angle1=70;
  angle2=62;
  angle3=16;
  flag_qr=false;
  }

 
 if (b_button) {// сложение севроприводов в домашнее положение
  angle1=235;
  angle2=62;
  angle3=90;} 

///// МЕСТНАЯ АВТОНОМКА /////
if (x_button && !auto_mode){
  auto_mode=true;
  auto_timer=millis();
  digitalWrite(M1_INA, HIGH);
  digitalWrite(M1_INB, LOW);
  analogWrite(M1_PWM, 254);

  digitalWrite(M2_INA, LOW);
  digitalWrite(M2_INB, HIGH);
  analogWrite(M2_PWM, 254);
  
  

 }
 if ((auto_mode)&&(millis()-auto_timer>2000)&&(!flag_stop_up)&&(!flag_torm)&&(abs(mpu_roll)<14)){flag_stop_up=true;time_stop_up=millis();Serial.println("first_stop");}


//
if ((auto_mode)&&(!flag_torm)&&(flag_stop_up)&&(millis()-time_stop_up>300)&&((mpu_roll)<14)){

  flag_torm=true; torm_timer=millis();//auto_mode=false; flag_start_up=false;// тормозим millis()-time_from_up
  digitalWrite(M1_INA, HIGH);
  digitalWrite(M1_INB, HIGH);
  analogWrite(M1_PWM, 255);
  Serial.println("STOP SToooooooooooop");
  digitalWrite(M2_INA, HIGH);
  digitalWrite(M2_INB, HIGH);
  analogWrite(M2_PWM, 255);

}

if ((flag_torm)&&(millis()-torm_timer>300)&&(auto_mode)){
  auto_mode=false; flag_stop_up=false; flag_torm=false; 
  Serial.println("TORMOS STOP");
  
}


if (auto_mode){Serial.println(mpu_roll);}
}




 
