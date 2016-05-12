#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(81);

const char *ssid = "RemoteControl";
const char *password = "remotecontrol";

#define STS_PIN 13
#define GPIO12  12

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch(type) {
    case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    digitalWrite(STS_PIN, LOW);
    break;
    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      digitalWrite(STS_PIN, HIGH);
      // send message to client
      webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
    Serial.printf("[%u] get Text: %s\n", num, payload);
    command_handle(payload, lenght);
    // send message to client
    // webSocket.sendTXT(num, "message here");

    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;
    case WStype_BIN:
    Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
    hexdump(payload, lenght);

    // send message to client
    // webSocket.sendBIN(num, payload, lenght);
    break;
  }

}

void command_handle(uint8_t *payload, size_t lenght) {

  // uint8_t io;
  // bool io_state;
  // String command;
  //
  // command.toCharArray((char *)payload,8);
  //
  // //GPIO12_1
  // //if (command.startsWith("GPIO")) {
  //     //cmd.substring(4,6);
  //     //cmd.substring(7,8);
  //     Serial.println(command.substring(4,6));
  //     Serial.println(command.substring(7,8));
  // //}

}

void setup() {

  Serial.begin(115200);

  pinMode(STS_PIN, OUTPUT);
  pinMode(GPIO12, OUTPUT);

  digitalWrite(STS_PIN, LOW);
  digitalWrite(GPIO12, LOW);

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.softAP(ssid, password);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
