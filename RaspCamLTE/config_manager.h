#ifndef __RASPCAM_CONFIG_MANAGER__
#define __RASPCAM_CONFIG_MANAGER__

#include <stdio.h>
#include <Wire.h>


/* CONSTANT DECLARATION
*  24C32 EEPROM Address on RTC DS3231 module
*  
* 
*/
//Page addresses on 24C32 EEPROM
#define EEPROM_ADR 0x57
#define START_TIME_EEPROM_ADDR 0x00
#define ON_DURATION_EEPROM_ADDR 0x32


void loadConfigFromEEPROM();
void writeConfigOnDuration(uint8_t onDuration);
void writeConfigStartTime(uint8_t startHour, uint8_t startMinute);

void initI2CEEPROM();
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, uint8_t data );
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, uint8_t* data, uint8_t length );
uint8_t i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress );
void i2c_eeprom_read_page( int deviceaddress, unsigned int eeaddress, uint8_t *buffer, int length );


#endif __RASPCAM_CONFIG_MANAGER__