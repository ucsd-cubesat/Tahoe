#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "WiFiNINAConnect.h"
#include "LSM9DS1.h"
#include "LSM9DS1_registers.h"
#include "DS18B20.h"
#include "HTTP.h"

//Accelerometer settings
const uint32_t BUS_FREQ = 10000000;
const pin_size_t CS_LSM9DS1_XL = 10;
const pin_size_t CS_LSM9DS1_MG = 9;
LSM9DS1 accelDevice(BUS_FREQ, CS_LSM9DS1_XL, CS_LSM9DS1_MG);
//SPIDevice accel = accelDevice.returnXL();
int16_t x; float x_float;
int16_t y; float y_float;
int16_t z; float z_float;

//HTTP settings
WiFiClient client;
int    HTTP_PORT   = 8080;
String HTTP_METHOD = "GET ";
//char   HOST_NAME[] = "tritoncubed.us-east-2.elasticbeanstalk.com";
IPAddress server(192, 168, 1, 107); 
char   HOST_NAME[] = "192.168.1.107";
String PATH_NAME   = "/launchapi/ping";

//Temperature Probe settings
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
float tempC;


void setup() {
  beginWiFiConnect();
  Serial.println(accelDevice.rebootXLG());
  accelDevice.enterXLOnlyMode(ODR_XL_238, FS_XL_4);
  setupDS18B20(insideThermometer, sensors); 
  delay(2500); 
}

void loop() {
  accelDevice.readAccelerometer(x, y, z);
  accelDevice.readAccelerometer(x_float, y_float, z_float);
  Serial.print("Acceleration: " + String(x_float) + " " + String(y_float) + " " + String(z_float));
  Serial.println();
  delay(2500);

  tempC = sensors.getTempC(insideThermometer);
  loopTempQuery(insideThermometer, sensors);
  delay(2500);

  Payload payload = createPayload(x, y, z, tempC);
  /*serverConnect(server, HTTP_PORT, client);
  delay(2500);
  httpPost(HTTP_METHOD, payload, PATH_NAME, HOST_NAME, client);
  delay(7500); */
  if (client.connect(server, 8080)) {
    client.println("POST /launchapi/binary HTTP/1.1");
    client.println("Host: 192.168.1.107");
    client.println("Content-Length: 10"); // Should be sizeof payload
    client.println("Connection: close");
    client.println();
    client.write((uint8_t*) &payload, sizeof(payload));
    Serial.println("Sent 8 bytes, response: ");

    for (int i = 0; i < 5 && !client.available(); i++) {
      delay(100);
    }
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    Serial.println(); 
  }
  delay(10000);
}






