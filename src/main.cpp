/*

  Repeating Wifi Web Client

 This sketch connects to a a web server and makes a request

 using a WiFi equipped Arduino board.

 created 23 April 2012

 modified 31 May 2012

 by Tom Igoe

 modified 13 Jan 2014

 by Federico Vanzati

 http://www.arduino.cc/en/Tutorial/WifiWebClientRepeating

 This code is in the public domain.

 */

#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "Madrigal";
char pass[] = "6c75696769";

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
IPAddress server(192, 168, 0, 99);

void setup() {

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  // check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);

  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  do {
    WiFi.begin(ssid, pass);
    delay(5000);
    status = WiFi.status();
  } while (status != WL_CONNECTED);
  Serial.println("Connected.");
}

  /**
   * Beware of the built-in types!
   * For this arduino, int is 16 bytes.
   * For exact C to Java types, use this table:
   * 
   * Java   ->   C
   * --------------------
   * byte   ->   int8_t
   * short  ->   int16_t
   * int    ->   int32_t
   * long   ->   int64_t
   * float  ->   float
   * double ->   double
   * char   ->   uint16_t
   * String ->   char[]
   * 
   * Strings should be stored in fixed-size character arrays.
   * 0-padding should be used in left-over space.
   * 
   * This ensures that the same-sized packet is sent each time,
   * to prevent as much formatting and encoding as possible.
   */
  typedef struct {
    int32_t A;
    float B;
    char C[32];
  } Payload;

void loop() {

  // make raw data
  Payload payload;
  payload.A = 1;
  payload.B = 3.14159F;
  strcpy(payload.C, "This is a short string.");

  // Send the raw data
  if (client.connect(server, 8080)) {
    client.println("POST /launchapi/binary HTTP/1.1");
    client.println("Host: 192.168.0.99");
    client.println("Content-Length: 40"); // Should be sizeof payload
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
  client.stop();

  delay(postingInterval);
}