#include <stdio.h>
#include "misc.h"
#include "config_manager.h"

bool isRouterOn = false;
bool isRaspOn = false;
unsigned int cntWaitForRouterOn = 0;
unsigned int cntNumRaspOn = 0;

extern RTC_DS3231 rtc;
/*
 * DateTime startTime = DateTime(2022, 8, 25, 02, 48, 00); is the time start to collect data
 * 
*/
DateTime startTime = DateTime(2022, 9, 4, 20, 20, 00);

/*
 * onRaspDuration = TimeSpan(0, 0, 3, 0) means the Raspberry will stay ON for 3 minutes, then it is set to OFF
 * offRaspDuration = TimeSpan(0, 0, 0, 5); means the Raspberry will stay OFF for 5 seconds, then it is set to ON
*/
TimeSpan onRaspDuration = TimeSpan(0, 0, 11, 0);
TimeSpan offRaspDuration = TimeSpan(0, 0, 0, 5);


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
  Serial.print("DEBUG - ");
  Serial.print(str);
  #endif

  }

void SerialDebugPrint(const char* str){
  #ifdef SERIAL_DEBUG
  if (strcmp(str, "\r\n") != 0 and strcmp(str, "\n\r") != 0) {
    Serial.print("DEBUG - ");
  }  
  Serial.print(str);
  #endif
  }

void SerialDebugPrint(StringSumHelper& str){
    #ifdef SERIAL_DEBUG
    Serial.print("DEBUG - ");
    Serial.print(str);
    #endif
  }
  
void SerialDebugPrint(int num){
    #ifdef SERIAL_DEBUG
    Serial.print("DEBUG - ");
    Serial.print(num);
    #endif
  }

  
void SerialDebugPrint(unsigned int num){
     #ifdef SERIAL_DEBUG
     Serial.print("DEBUG - ");
    Serial.print(num);
    #endif
  }
  
void SerialDebugPrint(char c){
  #ifdef SERIAL_DEBUG
  Serial.print("DEBUG - ");
  Serial.print(c);
  #endif  
  }

void SerialDebugPrint(bool boolVal){
  #ifdef SERIAL_DEBUG
  Serial.print("DEBUG - ");
  Serial.print(boolVal);
  #endif  
  }

void SerialReponse(char* buff ) {
  Serial.print("R");
  Serial.print(buff);
  Serial.print("\r\n");
}

void SerialReponse(const char* buff ) {
  Serial.print("R");
  Serial.print(buff);
  Serial.print("\r\n");
}

bool setHourAlarm1(uint8_t setHour, uint8_t setMinute) {
  DateTime _setStartTime = DateTime(startTime.year(), startTime.month(), startTime.day(), setHour, setMinute, 0);
  startTime = _setStartTime;
  bool _result = false;
  if(!rtc.setAlarm1(
              _setStartTime,
              DS3231_A1_Hour
            )) {
                SerialDebugPrint("Error, alarm wasn't set!\n");
            } else {
                SerialDebugPrint("Alarm will happen at " + DateTimeToString(_setStartTime) + "\r\n");
                _result = true;
            }

  return _result;
}

void SerialReadCmdRoutine(){
   
    if (Serial.available() > 0) {
      String incomingStr = Serial.readString();
      incomingStr.trim();     
      //SerialDebugPrint(incomingStr);

      int cmd = incomingStr[0];
      int incomingStrLen = incomingStr.length();

      switch (cmd) {
        case 's':
        // s 1 6  2  9
        // 0 1 2  3  4
        if (incomingStrLen == 5) {
            unsigned int startHour, startMinute;
            startHour   = (incomingStr[1] - '0')*10 + (incomingStr[2] - '0');
            startMinute = (incomingStr[3] - '0')*10 + (incomingStr[4] - '0');
    
            startTime = DateTime(startTime.year(), startTime.month(), startTime.day(), startHour, startMinute, 0);

            uint8_t eepbuff[2] = {(uint8_t)startHour, (uint8_t)startMinute};
            writeConfigStartTime(startHour, startMinute);
            
            rtc.clearAlarm(1);
            setHourAlarm1(startHour, startMinute);            
    
            initOnNewDay();

            SerialDebugPrint("Successfully set Alarm1 = ");
            SerialDebugPrint(startHour);
            SerialDebugPrint(startMinute);
            SerialDebugPrint("\r\n");

            SerialReponse("0");
            
          } else {
              SerialDebugPrint("Invalid set StartTime command\r\n");   
              SerialReponse("1");     
          }
        
        break;

        case 'n':        
        // n
        // n 0 1
        int onMinutes;
        if (incomingStrLen == 3) {
          onMinutes = (incomingStr[1] - '0')*10 + (incomingStr[2] - '0');
          onRaspDuration = TimeSpan(0, 0, onMinutes, 0);
          writeConfigOnDuration(onMinutes);          
          SerialDebugPrint("Successfully set onRaspDuration = ");
          SerialDebugPrint(onMinutes);
          SerialDebugPrint("\r\n");
          SerialReponse("0");
          } else {
              SerialDebugPrint("Invalid set onRaspDuration command\r\n");    
              SerialReponse("1");  
            }
            
        break;

        case 't':
        if (incomingStrLen == 1) {
          float f_temp = rtc.getTemperature();
          String tempStr(f_temp, 1);                    

          SerialDebugPrint(tempStr);
          SerialDebugPrint("\r\n");

          SerialReponse(tempStr.c_str());

        }
        break;

        case 'i':
        if (incomingStrLen == 1) {
          float f_temp = rtc.getTemperature();  
          String tempStr(f_temp, 1);        
          char reponseBuff[50];
          DateTime dt = rtc.now();
          sprintf(reponseBuff, "%02d:%02d:%02d-%02d/%02d/%02d,%s,%02d:%02d,%02d",  
          dt.hour(), dt.minute(), dt.second(), dt.day(), dt.month(), dt.year(), tempStr.c_str(), startTime.hour(), startTime.minute(), onRaspDuration.minutes());  
          SerialReponse(reponseBuff);

        }
        break;
        
      }
            
    }
      
  }
