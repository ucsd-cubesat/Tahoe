#ifndef HTTP_h
#define HTTP_h

#include <Arduino.h>
#include <WiFiNINA.h>

//Structure of the payload. Essentially contains all the different data to be sent
typedef struct {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    int16_t mag_x;
    int16_t mag_y;
    int16_t mag_z;
    float temp_C;
} Payload;

/**
 * @brief Function that returns a new payload with all the parameters set to sensor readings
 */
Payload createPayload(int16_t x, int16_t y, int16_t z, int16_t x_g, int16_t y_g, int16_t z_g, 
  int16_t x_m, int16_t y_m, int16_t z_m, float temp);

/**
 * @brief Creates an HTTP request that sends the payload to designated url
 */
void httpPost(IPAddress server , int HTTP_PORT, String HTTP_METHOD, Payload payload, 
    String PATH_NAME, char HOST_NAME[], WiFiClient client);

#endif