#include <Arduino.h>
#include <sparkbotServer.h>

String ssid = "NAA-TECHNO";
String password = "ZYSKQwz@#";

void setup()
{
    Serial.begin(9600);
    startSparkbot(ssid,password);
}

void loop()
{
    Serial.println("Main loop çalışıyor...");
    delay(2000);
}
