#include <SPI.h>

int slaveSelect = 10;

int delayTime = 50;

boolean data[32];
uint8_t regis;
uint16_t meg;
byte cmd;

void setup() {
  Serial.begin(115200);
  pinMode(slaveSelect, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);   
}

int value = 0x10;
void loop() {
    WriteMessage(0b00000000,0b1111111111111111,0b0000);
 //   Serial.println((0b1000 <<4) + 0b0001);
    delay(delayTime);     
}

void WriteMessage(uint8_t regis, uint32_t message, uint32_t cmd){
  uint32_t writeCMD = 0b1;
  //Serial.println(writeCMD<<31,BIN);
  uint32_t datas = writeCMD<<31 +message<<8 + cmd;
//  for(int i=31;i>=0;i--){
//    Serial.print(bitRead(datas,i));
//  }
  Serial.println(writeCMD<<(17)+0b1111111111111111,BIN);
  digitalWrite(slaveSelect,LOW);
  SPI.transfer(regis);
  SPI.transfer16(message);
  SPI.transfer(cmd + 0b1110);
  digitalWrite(slaveSelect,HIGH);
}

//boolean * CRC(){
//  boolean padded_frame[32] = {data};
//  boolean frame_crc[4] = {1,1,1,1};
//  boolean inv;
//  for(int i=31;i>=0;i=i-1){
//    inv = pow(padded_frame[i], frame_crc[3]);
//    frame_crc[3] = frame_crc[2];
//    frame_crc[2] = frame_crc[1];
//    frame_crc[1] = pow(frame_crc[0],inv);
//    frame_crc[0] = inv;
//    
//  }
//  for(int i=0; i<4;i++){
//    Serial.print(frame_crc[i]);   
//  }
//  Serial.println();
//
//  return frame_crc;
//}

byte calculate_CRC(uint32_t data){
  byte frame_crc = 0xF;
  boolean inv;
  for(int i=0;i<=31;i++){
    inv = pow(bitRead(data,i),bitRead(frame_crc,0));
    bitWrite(frame_crc,0,bitRead(frame_crc,1));
    bitWrite(frame_crc,1,bitRead(frame_crc,2));
    bitWrite(frame_crc,2,pow(bitRead(frame_crc,3),inv));
    bitWrite(frame_crc,3,inv);
  }
  for(int i=0;i<=3;i++){
    Serial.print(bitRead(frame_crc,i));
  }
  Serial.println();
  return frame_crc;
}
