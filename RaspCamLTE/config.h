  #ifndef __RASPCAMLTE_CONFIG__
  #define __RASPCAMLTE_CONFIG__
  
/*----- BEGIN CONFIG SECTION ----- */

// Enable (Uncomment) to AJUST RTC TIME = COMPILATION TIME
//#define ADJUST_RTC_TIME

// Enable (Uncomment) to allow serial.print
#define SERIAL_DEBUG

// Number of SECONDS need to wait for the router ON
#define WAITING_TIME_FOR_ROUTER 300

// Number of running cycles of Raspberry Pi after startTime
#define NUM_RUNNING_CYCLES 3

/*----- END CONFIG SECTION ----- */


/* BEGIN PIN CONFIG */
//SQW on RTC
#define InterruptPin 2
// 2 Relay Pins
#define RouterPin 4
#define RaspPin 6

#define STATUS_LED 13

/* END PIN CONFIG */

#endif __RASPCAMLTE_CONFIG__
