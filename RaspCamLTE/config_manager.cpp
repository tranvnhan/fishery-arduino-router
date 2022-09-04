#include "config_manager.h"
#include "misc.h"

extern TimeSpan onRaspDuration;

void loadConfigFromEEPROM(){
    // [0]StartHour    [1]StartMinute    [2]OnDuration
    byte eepbuff[3];
    i2c_eeprom_read_page(EEPROM_ADR, START_TIME_EEPROM_ADDR, &eepbuff[0], 2);
    i2c_eeprom_read_page(EEPROM_ADR, ON_DURATION_EEPROM_ADDR, &eepbuff[2], 1); 

    setHourAlarm1(eepbuff[0], eepbuff[1]);
    onRaspDuration = TimeSpan(0, 0, eepbuff[2], 0);
}

void writeConfigStartTime(uint8_t startHour, uint8_t startMinute) {

    uint8_t eepbuff[2] = {(uint8_t)startHour, (uint8_t)startMinute};
    i2c_eeprom_write_page(EEPROM_ADR, START_TIME_EEPROM_ADDR, (uint8_t *)eepbuff, 2);

}

void writeConfigOnDuration(uint8_t onDuration) {

    uint8_t eepbuff = onDuration;
    i2c_eeprom_write_page(EEPROM_ADR, ON_DURATION_EEPROM_ADDR, (uint8_t *)&eepbuff, 1);

}

void initI2CEEPROM() {
   Wire.begin();
  }

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, uint8_t data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, uint8_t* data, uint8_t length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}

uint8_t i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

void i2c_eeprom_read_page( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}