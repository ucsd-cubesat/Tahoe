#ifndef HTTP_h
#define HTTP_h

#include <Arduino.h>
#include <WiFiNINA.h>

typedef struct {
    int16_t accel_X;
    int16_t accel_Y;
    int16_t accel_Z;
    float temp_C;
    
} Payload;

void serverConnect(IPAddress server, int HTTP_PORT, WiFiClient client);

Payload createPayload(int16_t x, int16_t y, int16_t z, float temp);

void httpPost(String HTTP_METHOD, Payload payload, String PATH_NAME, char HOST_NAME[], WiFiClient client);

#endif