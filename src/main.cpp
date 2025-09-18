#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h> // https://github.com/Links2004/arduinoWebSockets
#include <serverFlash.h>
const char *ssid = "NAA-TECHNO";
const char *password = "ZYSKQwz@#";
const char *host = "sparkbot.correcttechno.com";
const int port = 321; // WSS default port

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
        Serial.println("Connected to WSS server");
        webSocket.sendTXT("robot_to_user|qwerty|abc|Merhaba kullanıcı");
        break;
    case WStype_TEXT:
    {
        String message = String((char *)payload);
        if (message == "update")
            otaUpdate("otoupdate");
    }
    break;
    case WStype_DISCONNECTED:
        Serial.println("Disconnected");
        break;
    default:
        Serial.print("");
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");

    // WSS için
    webSocket.beginSSL(host, port, "/");
    webSocket.onEvent(webSocketEvent);
}

void loop()
{
    webSocket.loop();
}
