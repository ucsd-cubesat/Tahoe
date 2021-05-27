#include "DS18B20.h"
// Data wire is plugged into port 2 on the Arduino
//#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
//DallasTemperature sensors(&oneWire);
// arrays to hold device addresses
//DeviceAddress insideThermometer;
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress, DallasTemperature sensors)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}
void printAlarms(uint8_t deviceAddress[], DallasTemperature sensors)
{
  char temp;
  temp = sensors.getHighAlarmTemp(deviceAddress);
  Serial.print("High Alarm: ");
  Serial.print(temp, DEC);
  Serial.print("C/");
  Serial.print(DallasTemperature::toFahrenheit(temp));
  Serial.print("F | Low Alarm: ");
  temp = sensors.getLowAlarmTemp(deviceAddress);
  Serial.print(temp, DEC);
  Serial.print("C/");
  Serial.print(DallasTemperature::toFahrenheit(temp));
  Serial.print("F");
}
// main function to print information about a device
void printData(DeviceAddress deviceAddress, DallasTemperature sensors)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress, sensors);
  Serial.println();
}
void checkAlarm(DeviceAddress deviceAddress, DallasTemperature sensors)
{
  printData(deviceAddress, sensors);
  if (sensors.hasAlarm(deviceAddress))
  {
    Serial.print("ALARM!");
    Serial.println();
  }
}
void setupDS18B20(DeviceAddress insideThermometer, DallasTemperature sensors) {
  Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin();
  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  // search for devices on the bus and assign based on an index.
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();
  Serial.print("Device 0 Alarms: ");
  printAlarms(insideThermometer, sensors);
  Serial.println();
  Serial.println("Setting alarm temps...");
  // alarm when temp is higher than 30C
  sensors.setHighAlarmTemp(insideThermometer, 30);
  // alarm when temp is lower than -10C
  sensors.setLowAlarmTemp(insideThermometer, -10);
  Serial.print("New Device 0 Alarms: ");
  printAlarms(insideThermometer, sensors);
  Serial.println();
  
}

void loopTempQuery(DeviceAddress insideThermometer, DallasTemperature sensors) {
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  checkAlarm(insideThermometer, sensors);
}
