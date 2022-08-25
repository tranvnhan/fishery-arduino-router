/* Example implementation of an alarm using DS3231
 *
 * VCC and GND of RTC should be connected to some power source
 * SDA, SCL of RTC should be connected to SDA, SCL of arduino
 * SQW should be connected to CLOCK_INTERRUPT_PIN
 * CLOCK_INTERRUPT_PIN needs to work with interrupts
 * ref: https://github.com/adafruit/RTClib/blob/master/examples/DS3231_alarm/DS3231_alarm.ino
 */

#include "misc.h"

RTC_DS3231 rtc;

extern bool isRouterOn;
extern bool isRaspOn;
extern unsigned int cntWaitForRouterOn;
extern unsigned int cntNumRaspOn;

void setup() {

    Serial.begin(9600);
    
    pinMode(RouterPin, OUTPUT);
    pinMode(RaspPin, OUTPUT);

    initOnNewDay();

    // initializing the rtc
    if(!rtc.begin()) {      
        Serial.flush();
        SerialDebugPrint("Couldn't find RTC!\n");
        while (1) delay(10);
    }

    #ifdef ADJUST_RTC_TIME   
    // this will adjust to the date and time at compilation
    DateTime compilationTime = DateTime(F(__DATE__), F(__TIME__));
    //Serial.println("Set time RTC = " + DateTimeToString(compilationTime));
    rtc.adjust(compilationTime);
    #endif

    //we don't need the 32K Pin, so disable it
    rtc.disable32K();

    // Making it so, that the alarm will trigger an interrupt
    pinMode(InterruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(InterruptPin), onAlarm, FALLING);

    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);

    if(!rtc.setAlarm1(
          startTime,
          DS3231_A1_Hour // this mode triggers the alarm when the seconds match. See Doxygen for other options
    )) {
        SerialDebugPrint("Error, alarm wasn't set!\n");
    }else {
        SerialDebugPrint("Alarm will happen at " + DateTimeToString(startTime) + "\n");
    }
 
}

void loop() {
    // print current time
    char date[10] = "hh:mm:ss";
    rtc.now().toString(date);
    Serial.print(date);
    // the value at SQW-Pin (because of pullup 1 means no alarm)
    Serial.print(" SQW: ");
    Serial.print(digitalRead(InterruptPin));
    // whether a alarm happened happened
    Serial.print(" Alarm1: ");
    Serial.print(rtc.alarmFired(1));
    Serial.print("\n");
    
    Serial.print(" Alarm2: ");
    Serial.print(rtc.alarmFired(2));
    Serial.print("\n"); 
    // Serial.print(" Alarm2: ");
    // Serial.println(rtc.alarmFired(2));
    // control register values (see https://datasheets.maximintegrated.com/en/ds/DS3231.pdf page 13)
    // Serial.print(" Control: 0b");
    // Serial.println(read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL), BIN);

    // resetting SQW and alarm 1 flag

    // using setAlarm1, the next alarm could now be configurated
    if(rtc.alarmFired(1)) {

        if (isRouterOn && cntWaitForRouterOn == 0) {

          //Delay for routerOn, internet on
          while (cntWaitForRouterOn++ < WAITING_TIME_FOR_ROUTER) {
            delay(1000);
            Serial.print("Wait for the router ... ");
            Serial.print(cntWaitForRouterOn);
            Serial.println();
          }

          digitalWrite(RaspPin, HIGH);
          isRaspOn = true;
         }
         
         rtc.clearAlarm(1);

         if (isRaspOn) {

            // If number of cycles is enough on this day
            if (cntNumRaspOn < NUM_RUNNING_CYCLES) {
              //Set Alarm1 to OFF the Ras after onRaspDuration
              DateTime alarm1Time = rtc.now() + onRaspDuration;
              Serial.println("Alarm1 is set to " + DateTimeToString(alarm1Time) + " OFF Rasp");
              rtc.setAlarm1(alarm1Time, DS3231_A1_Hour);    
            } else {
              // Initialize for new day
              initOnNewDay();
              //Set timer for the following days, same as (hour, minute) in startTime
              DateTime alarm1Time = startTime;
              Serial.println("Alarm1 is set to " + DateTimeToString(alarm1Time) + " ON Rasp");
              rtc.setAlarm1(alarm1Time, DS3231_A1_Hour);    
            }

         }
         else {
            //Set Alarm1 to ON the Ras after offRaspDuration
            DateTime alarm1Time = rtc.now() + offRaspDuration;
            Serial.println("Alarm1 is set to " + DateTimeToString(alarm1Time) + " ON Rasp");
            rtc.setAlarm1(alarm1Time, DS3231_A1_Hour);    
         }
         
    }

    delay(1000);
}

void onAlarm() {
   
    if(isRouterOn == false){
      isRouterOn = true; 
      digitalWrite(RouterPin, HIGH);
      Serial.println("ON_Router Relay 1");    
    }
    else
    {
      //When Router is ON and may have internet, keep trying to periodically toggle Rasp PIN by Alarm1
      if (isRaspOn)  {
          isRaspOn = false; 
          digitalWrite(RaspPin, LOW);
          Serial.println("OFF_RaspPin Relay 2");          
        }
      else {
          isRaspOn = true; 
          digitalWrite(RaspPin, HIGH);
          Serial.println("ON_RaspPin Relay 2");   
          cntNumRaspOn++;
          Serial.print("NumRaspOn ");
          Serial.print(cntNumRaspOn);
          Serial.println();
        }     
    } 
     
}
