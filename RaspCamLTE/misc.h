  #ifndef __RASPCAMLTE_MISC__
  #define __RASPCAMLTE_MISC__

  #include <RTClib.h>
  #include "config.h"


  bool setHourAlarm1(uint8_t setHour, uint8_t setMinute);

  String DateTimeToString(DateTime dt);
  void initOnNewDay();

  void SerialDebugPrint(String str);
  void SerialDebugPrint(const char* str);
  void SerialDebugPrint(StringSumHelper&);
  void SerialDebugPrint(int num);
  void SerialDebugPrint(unsigned int num);
  void SerialDebugPrint(char c);
  void SerialDebugPrint(bool);

  void SerialReponse(char* buff );
  void SerialReponse(const char* buff );

  void SerialReadCmdRoutine();
  

  #endif __RASPCAMLTE_MISC__
