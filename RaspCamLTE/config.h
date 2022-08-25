  #ifndef __RASPCAMLTE_CONFIG__
  #define __RASPCAMLTE_CONFIG__
  
/*----- BEGIN CONFIG SECTION ----- */

// Enable to AJUST RTC TIME = COMPILATION TIME
//#define ADJUST_RTC_TIME
#define SERIAL_DEBUG

// Number of SECONDS need to wait for the router ON
#define WAITING_TIME_FOR_ROUTER 300

// Number of running cycles of Raspberry Pi after startTime
#define NUM_RUNNING_CYCLES 11

/*
 * DateTime startTime = DateTime(2022, 8, 25, 02, 48, 00); is the time start to collect data
 * 
*/
const DateTime startTime = DateTime(2022, 8, 24, 17, 50, 00);

/*
 * onRaspDuration = TimeSpan(0, 0, 3, 0) means the Raspberry will stay ON for 3 minutes, then it is set to OFF
 * offRaspDuration = TimeSpan(0, 0, 0, 5); means the Raspberry will stay OFF for 5 seconds, then it is set to ON
*/
const TimeSpan onRaspDuration = TimeSpan(0, 0, 7, 0);
const TimeSpan offRaspDuration = TimeSpan(0, 0, 0, 5);

/*----- END CONFIG SECTION ----- */



/* BEGIN PIN CONFIG */
//SQW on RTC
#define InterruptPin 2
// 2 Relay Pins
#define RouterPin 4
#define RaspPin 6

/* END PIN CONFIG */

#endif __RASPCAMLTE_CONFIG__
