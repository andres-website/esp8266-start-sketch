#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>

void setup() {
  Serial.begin(115200);

  // Инициализация Wi-Fi и OTA
  WiFiManager wifiManager;
  
  // Создание объекта для хранения настроек
  WiFiManagerParameter custom_ip("ip", "Static IP", "", 15);
  WiFiManagerParameter custom_gateway("gateway", "Gateway", "", 15);
  WiFiManagerParameter custom_subnet("subnet", "Subnet", "", 15);
  WiFiManagerParameter custom_dns("dns", "DNS", "", 15);

  // Добавление параметров в WiFiManager
  wifiManager.addParameter(&custom_ip);
  wifiManager.addParameter(&custom_gateway);
  wifiManager.addParameter(&custom_subnet);
  wifiManager.addParameter(&custom_dns);

  // Запуск WiFiManager
  if (!wifiManager.autoConnect("ESP8266AP")) {
    Serial.println("Failed to connect and hit timeout");
    // Сделайте что-то здесь, если подключение не удалось
    // Например, перезагрузка устройства
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println("Connected to Wi-Fi!");

  // Если выбран Static IP, устанавливаем заданные параметры
  if (WiFi.SSID() != "") {
    Serial.println("Connected with IP: " + WiFi.localIP().toString());
    if (custom_ip.getValue() != "") {
      IPAddress staticIP, gateway, subnet, dns;
      staticIP.fromString(custom_ip.getValue());
      gateway.fromString(custom_gateway.getValue());
      subnet.fromString(custom_subnet.getValue());
      dns.fromString(custom_dns.getValue());

      WiFi.config(staticIP, gateway, subnet, dns);
      Serial.println("Static IP configured");
    }
  }

  // Инициализация OTA
  ArduinoOTA.onStart([]() {
    Serial.println("OTA update started");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA update finished");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
}

void loop() {
  // Обработка OTA
  ArduinoOTA.handle();

  // Дополнительный код для вашей программы может быть добавлен здесь
}
