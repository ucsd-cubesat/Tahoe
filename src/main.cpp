#include <Arduino.h>
#include <SPI.h>
// #include <WiFiNINA.h>

// #include "WiFiNINAConnect.h"
#include "LSM9DS1.h"
#include "LSM9DS1_registers.h"
// #include "DS18B20.h"
// #include "HTTP.h"

// #include "MAXM8C.h"
// #include "NMEA.h"
// #include <UART.h>

//Accelerometer settings
const uint32_t BUS_FREQ = 10000000;
// const pin_size_t CS_LSM9DS1_XL = 10;
// const pin_size_t CS_LSM9DS1_MG = 9;

#define CS_LSM9DS1_XL 10
#define CS_LSM9DS1_MG 9
LSM9DS1 accelDevice(BUS_FREQ, CS_LSM9DS1_XL, CS_LSM9DS1_MG);

//variables to store normalized and non-normalized accelerations
int16_t x; float x_float;
int16_t y; float y_float;
int16_t z; float z_float;

//variables to store gyro readings
int16_t x_g; float x_g_f;
int16_t y_g; float y_g_f;
int16_t z_g; float z_g_f;

//variables to store magnetic readings
int16_t x_m; float x_m_f;
int16_t y_m; float y_m_f;
int16_t z_m; float z_m_f;

// //HTTP settings
// WiFiClient client;
// int    HTTP_PORT   = 8080;
// String HTTP_METHOD = "POST ";
// //char   HOST_NAME[] = "tritoncubed.us-east-2.elasticbeanstalk.com";
// IPAddress server(192, 168, 0, 99); 
// char   HOST_NAME[] = "192.168.1.107";
// String PATH_NAME   = "/launchapi/db";

// //Temperature Probe settings
// #define ONE_WIRE_BUS 2
// OneWire oneWire(ONE_WIRE_BUS);
// DallasTemperature sensors(&oneWire);
// DeviceAddress insideThermometer;
// float tempC;

// // GPS
// MAXM8C gps;
// char LINEBUFF[100];
// char *utc, *lat, *lat_ns, *lon, *lon_ew;

void configureXLInterrupts(LSM9DS1* accelDevice);

void setup() {
  SPI.begin();
  Serial1.begin(9600);
  // beginWiFiConnect();
  Serial.println(accelDevice.rebootXLG());
  accelDevice.enterXLGMode(ODR_G_952, FS_G_2000, FS_XL_4);
  Serial.println(accelDevice.rebootMag());
  accelDevice.configureMag(OPER_M_CONT, ODR_M_40, POWER_M_MED, FS_M_8);

  configureXLInterrupts(&accelDevice);

  // setupDS18B20(insideThermometer, sensors);
}

void loop() {
  // while (1) {
  //   gps.update(Serial1);
  //   if (gps.available()) {
  //     gps.read(LINEBUFF, sizeof(LINEBUFF));
  //     if (nmea_get_type(LINEBUFF) == NMEA::NMEA_GGA) {
  //       Serial.println(LINEBUFF);
  //       nmea_parse_gga(LINEBUFF, &utc, &lat, &lat_ns, &lon, &lon_ew, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
  //       break;
  //     }
  //   }
  // }

  //Requests unnormalized and normalized values of acceleration
  accelDevice.readAccelerometer(x, y, z);
  x_float = LSM9DS1::convertRaw(x, FS_XL_4);
  y_float = LSM9DS1::convertRaw(y, FS_XL_4);
  z_float = LSM9DS1::convertRaw(z, FS_XL_4);
  //accelDevice.readAccelerometer(x_float, y_float, z_float);
  //Outputs normalized values to Serial monitor
  Serial.print("Acceleration(m/s^2): " + String(x_float) + " " + String(y_float) + " " + String(z_float));
  Serial.println();

  //Requests values of angular velocity and outputs to Serial monitor
  accelDevice.readGyroscope(x_g, y_g, z_g);
  x_g_f = LSM9DS1::convertRaw(x_g, FS_G_2000);
  y_g_f = LSM9DS1::convertRaw(y_g, FS_G_2000);
  z_g_f = LSM9DS1::convertRaw(z_g, FS_G_2000);
  //accelDevice.readGyroscope(x_g_f, y_g_f, z_g_f);
  Serial.print("Angular Velocity(deg/s): " + String(x_g_f) + " " + String(y_g_f) + " " + String(z_g_f));
  Serial.println();
  
  //Requests values of induction and outputs to Serial monitor
  accelDevice.readMagnetosensor(x_m, y_m, z_m);
  x_m_f = LSM9DS1::convertRaw(x_m, FS_M_8);
  y_m_f = LSM9DS1::convertRaw(y_m, FS_M_8);
  z_m_f = LSM9DS1::convertRaw(z_m, FS_M_8);
  //accelDevice.readMagnetosensor(x_m_f, y_m_f, z_m_f);
  Serial.print("Induction(gausses): " + String(x_m_f) + " " + String(y_m_f) + " " + String(z_m_f));
  Serial.println();

  // //Requests temperature in Celcius
  // tempC = sensors.getTempC(insideThermometer);
  // loopTempQuery(insideThermometer, sensors);

  // //Prepares a payload object to send through an HTTP request
  // Payload payload = createPayload(x, y, z, x_g, y_g, z_g, x_m, y_m, z_m, tempC, lat, *lat_ns, lon, *lon_ew);
  // httpPost(server, HTTP_PORT, HTTP_METHOD, payload, PATH_NAME, HOST_NAME, client);
}


void configureXLInterrupts(LSM9DS1* accelDevice) {

  // XYZHIE_XL: Interrupt on all high events on all axis
  // false: OR interrupts
  // duration: trigger for the duration of 5 samples (for ODR = 952 Hz, this is ~ 5ms)
  accelDevice->configXLInterrupt(XYZHIE_XL, false, 5);

  // set threshold to 20; raw accelerometer reading in mg/LBS
  accelDevice->setAllXLInterruptTHT(20);

  // set interrupt 1 to report gyroscope and accelerometer triggers
  // OR combines the gyroscope and accelerometer triggers
  // method defaults to active high and push-pull functionality
  accelDevice->configIntPin(INT1, INT_IG_XL | INT1_IG_G);

}