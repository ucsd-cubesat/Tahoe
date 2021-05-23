#include "HTTP.h"

void serverConnect(IPAddress server , int HTTP_PORT, WiFiClient client) {
  if(client.connect(server, HTTP_PORT)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("connection failed");
  }
}

Payload createPayload(int16_t x, int16_t y, int16_t z, float temp) {
    Payload payload;
    payload.accel_X = x;
    payload.accel_Y = y;
    payload.accel_Z = z;
    payload.temp_C = temp;
    return payload;
}

void httpPost(String HTTP_METHOD, Payload payload, String PATH_NAME, char HOST_NAME[], WiFiClient client) {
  client.println(HTTP_METHOD + PATH_NAME + " HTTP/1.1");
  client.println("Host: 192.168.1.107");
  client.println("Content-Length: 10"); // Should be sizeof payload
  client.println("Connection: close");
  client.println(); 
  client.write((uint8_t*) &payload, sizeof(payload));
  Serial.print("Bytes: ");

  while(client.available()) {
    // read an incoming byte from the server and print them to serial monitor:
    char c = client.read();
    Serial.write(c);
    Serial.println();
  }

  if(!client.connected()) { 
    // if the server's disconnected, stop the client:
    Serial.println("disconnected");
    client.stop();
  }
}