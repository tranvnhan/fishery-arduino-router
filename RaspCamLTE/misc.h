  #ifndef __RASPCAMLTE_MISC__
  #define __RASPCAMLTE_MISC__
  
  #include <RTClib.h>
  #include "config.h"

  String DateTimeToString(DateTime dt);
  void initOnNewDay();

  void SerialDebugPrint(String str);
  void SerialDebugPrint(const char* str);
  void SerialDebugPrint(StringSumHelper&);
  void SerialDebugPrint(int num);
  void SerialDebugPrint(unsigned int num);
  void SerialDebugPrint(char c);
  void SerialDebugPrint(String str, ...);

  #endif __RASPCAMLTE_MISC__
