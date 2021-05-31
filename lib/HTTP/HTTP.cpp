#include "HTTP.h"

Payload createPayload(int16_t x, int16_t y, int16_t z, int16_t x_g, int16_t y_g, int16_t z_g, 
  int16_t x_m, int16_t y_m, int16_t z_m, float temp, const char* lat, char lat_ns, const char* lon, char lon_ew) {
    Payload payload;
    payload.accel_x = x;
    payload.accel_y = y;
    payload.accel_z = z;
    payload.gyro_x = x_g;
    payload.gyro_y = y_g;
    payload.gyro_z = z_g;
    payload.mag_x = x_m;
    payload.mag_y = y_m;
    payload.mag_z = z_m;
    payload.temp_C = temp;
    strncpy(payload.lat, lat, sizeof(payload.lat));
    payload.lat_ns = lat_ns;
    strncpy(payload.lon, lon, sizeof(payload.lon));
    payload.lon_ew = lon_ew;
    return payload;
}

void httpPost(IPAddress server , int HTTP_PORT, String HTTP_METHOD, Payload payload, 
  String PATH_NAME, char HOST_NAME[], WiFiClient client) {
    if(client.connect(server, HTTP_PORT)) {
      Serial.println("Connected to server");

      client.println(HTTP_METHOD + PATH_NAME + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Content-Length: " + String(sizeof(payload)));
      client.println("Connection: close");
      client.println(); 
      client.write((uint8_t*) &payload, sizeof(payload));
      Serial.print("Bytes: ");

      for (int i = 0; i < 5 && !client.available(); i++) {
        delay(100);
      }

      while(client.available()) {
        // read an incoming byte from the server and print them to serial monitor:
        char c = client.read();
        Serial.write(c);
      }

      if(!client.connected()) { 
        // if the server's disconnected, stop the client:
        Serial.println("disconnected");
        client.stop();
      }
    }
    else {
      Serial.println("connection failed");
    }
}