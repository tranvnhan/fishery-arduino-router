/* Example implementation of an alarm using DS3231
 *
 * VCC and GND of RTC should be connected to some power source
 * SDA, SCL of RTC should be connected to SDA, SCL of arduino
 * SQW should be connected to CLOCK_INTERRUPT_PIN
 * CLOCK_INTERRUPT_PIN needs to work with interrupts
 * ref: https://github.com/adafruit/RTClib/blob/master/examples/DS3231_alarm/DS3231_alarm.ino
 */

#include <RTClib.h>
#include <LowPower.h>
RTC_DS3231 rtc;

//SQW on RTC
#define InterruptPin 2
// 2 Relay Pins
#define RouterPin 4
#define RaspPin 5

bool isOn = false;

void setup() {
    Serial.begin(9600);
    
    pinMode(RouterPin, OUTPUT);
    digitalWrite(RaspPin, LOW);
    pinMode(RaspPin, OUTPUT);
    digitalWrite(RouterPin, LOW);

    // initializing the rtc
    if(!rtc.begin()) {
        Serial.println("Couldn't find RTC!");
        Serial.flush();
        while (1) delay(10);
    }

    if(rtc.lostPower()) {
        // this will adjust to the date and time at compilation
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

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

    // rtc.setAlarm1(DateTime(2022, 07, 30, 17, 00, 00), DS3231_A1_Hour);
    if(!rtc.setAlarm1(
      DateTime(2022, 07, 30, 17, 48, 00), DS3231_A1_Hour
    )) {
      Serial.println("Error, alarm wasn't set!");
    }else {
      Serial.println("Alarm will happen at 5pm!");
    }
    // rtc.setAlarm2(DateTime(2022, 07, 30, 17, 48, 00), DS3231_A1_Hour);

    if(!rtc.setAlarm2(
      DateTime(2022, 07, 30, 17, 50, 00), DS3231_A2_Hour
    )) {
      Serial.println("Error, alarm wasn't set!");
    }else {
      Serial.println("Alarm will happen at 6pm!");
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
        rtc.clearAlarm(1);
        rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 1, 0), DS3231_A1_Hour);
        Serial.println("Alarm 1 cleared");
    }
    if(rtc.alarmFired(2)) {
        rtc.clearAlarm(2);
        rtc.setAlarm2(rtc.now() + TimeSpan(0, 0, 1, 0), DS3231_A2_Hour);
        Serial.println("Alarm 2 cleared");
    }

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
}

void onAlarm() {
    if(isOn == false){
      isOn = true; 
      digitalWrite(RaspPin, HIGH);
      digitalWrite(RouterPin, HIGH);
      Serial.println("ON_BC")    
    }
    else {
      isOn = false; 
      digitalWrite(RaspPin, LOW);
      digitalWrite(RouterPin, LOW);
      Serial.println("OFF_BC")  
    }  
}
