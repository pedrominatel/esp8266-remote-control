#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(81);

const char *ssid = "RemoteControl";
const char *password = "remotecontrol";
IPAddress server_ip(192,168,0,2);
IPAddress server_gw(192,168,0,1);
IPAddress server_sn(255,255,255,0);

#define BTN 12
#define LED 4

uint8_t btn_state = 0;
uint8_t btn_last = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch(type) {
    case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
    case WStype_CONNECTED:
    {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

      // send message to client
      webSocket.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
    Serial.printf("[%u] get Text: %s\n", num, payload);

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

void setup() {

  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  digitalWrite(LED, LOW);


  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.softAP(ssid, password);
  WiFi.config(server_ip, server_gw, server_sn);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  btn_state = digitalRead(BTN);

  if(btn_state != btn_last){
    if(btn_state == HIGH){
      digitalWrite(LED, HIGH);
      btn_last = HIGH;
      Serial.printf("High\n");
    } else {
      digitalWrite(LED, LOW);
      btn_last = LOW;
      Serial.printf("Low\n");
    }
  }
}
