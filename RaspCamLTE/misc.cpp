#include <stdio.h>
#include "misc.h"

  bool isRouterOn = false;
  bool isRaspOn = false;
  unsigned int cntWaitForRouterOn = 0;
  unsigned int cntNumRaspOn = 0;

 String DateTimeToString(DateTime dt) {
  char buf1[20];
  sprintf(buf1, "%02d:%02d:%02d %02d/%02d/%02d",  dt.hour(), dt.minute(), dt.second(), dt.day(), dt.month(), dt.year());
  return String(buf1);
}

void initOnNewDay() {
  isRouterOn = false;
  isRaspOn = false;
  cntWaitForRouterOn = 0;
  cntNumRaspOn = 0;
  digitalWrite(RaspPin, LOW);
  digitalWrite(RouterPin, LOW);
}

void SerialDebugPrint(String str){

  #ifdef SERIAL_DEBUG
  Serial.print(str);
  #endif
      
  }

void SerialDebugPrint(const char* str){
  #ifdef SERIAL_DEBUG
  Serial.print(str);
  #endif
  }

void SerialDebugPrint(StringSumHelper& str){
    #ifdef SERIAL_DEBUG
    Serial.print(str);
    #endif
  }
  
void SerialDebugPrint(int num){
    #ifdef SERIAL_DEBUG
    Serial.print(num);
    #endif
  }

  
void SerialDebugPrint(unsigned int num){
     #ifdef SERIAL_DEBUG
    Serial.print(num);
    #endif
  }
  
void SerialDebugPrint(char c){
  #ifdef SERIAL_DEBUG
  Serial.print(c);
  #endif  
  }
