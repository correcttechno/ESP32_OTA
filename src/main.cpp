#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "NAA-TECHNO";          // WiFi ismi
const char* password = "ZYSKQwz@#";  // WiFi şifresi

// HiveMQ public broker
const char* mqtt_server = "broker.hivemq.com";  
const int mqtt_port = 1883;

// OTA için firmware dosyasının bulunduğu HTTP adresi
const char* firmware_url = "https://correcttechno.com/firmware.bin";

WiFiClient espClient;
PubSubClient client(espClient);

void otaUpdate() {
  HTTPClient http;
  http.begin(firmware_url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    WiFiClient * stream = http.getStreamPtr();

    if (!Update.begin(contentLength)) {
      Serial.println("Update başlatılamadı!");
      return;
    }

    size_t written = Update.writeStream(*stream);
    if (written == contentLength) {
      Serial.println("Update tamamlandı!");
    } else {
      Serial.printf("Eksik yazıldı: %d / %d\n", written, contentLength);
    }

    if (Update.end()) {
      if (Update.isFinished()) {
        Serial.println("Başarılı! Yeniden başlatılıyor...");
        ESP.restart();
      } else {
        Serial.println("Update bitmedi!");
      }
    } else {
      Serial.printf("Update hatası: %s\n", Update.errorString());
    }
  } else {
    Serial.printf("HTTP hata kodu: %d\n", httpCode);
  }
  http.end();
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("MQTT Mesajı [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (message == "UPDATE") {
    Serial.println("OTA Başlatılıyor...");
    otaUpdate();
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT'ye bağlanıyor...");
    if (client.connect("ESP32Client")) {
      Serial.println("bağlandı!");
      client.subscribe("esp32/update");   // UPDATE komutu bu topic'ten gelecek
    } else {
      Serial.print("Hata, rc=");
      Serial.print(client.state());
      Serial.println(" 5 sn sonra tekrar denenecek...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi bağlı.");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
