#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>
#include <DallasTemperature.h>

void setupDS18B20(DeviceAddress insideThermometer, DallasTemperature sensors);
void checkAlarm(DeviceAddress deviceAddress, DallasTemperature sensors);
void loopTempQuery(DeviceAddress insideThermometer, DallasTemperature sensors);
#endif