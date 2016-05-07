#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager


#include <WebSocketsClient.h>
#include <Hash.h>

#define STATUS_PIN 13

WebSocketsClient webSocket;


void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {


    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                Serial.printf("[WSc] Connected to url: %s\n",  payload);

			    // send message to server when Connected
				webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);

            // send data to server
            // webSocket.sendBIN(payload, lenght);
            break;
    }

}

void setup() {

  Serial.begin(115200);

  pinMode(STATUS_PIN, OUTPUT);
  digitalWrite(STATUS_PIN, LOW);

  WiFiManager wifiManager;
  wifiManager.autoConnect("MyRemoteControl");

  Serial.println("Connected to the remote control server!");
  digitalWrite(STATUS_PIN, HIGH);

  webSocket.begin("192.168.0.123", 81);
  webSocket.onEvent(webSocketEvent);

}

void loop() {
  webSocket.loop();
}
