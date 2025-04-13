#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


void wifi_connect() {
  WiFiManager wifiManager;

  // Menentukan nama SSID yang akan ditampilkan saat tidak ada koneksi

  // Cek jika Wi-Fi tidak berhasil terkoneksi setelah timeout
  if (!wifiManager.autoConnect("Rival_Dual_Switch_AP")) {
    Serial.println("Fail ed to connect and hit timeout");
    ESP.reset();
    delay(1000);
  }
  Serial.println("Connected to Wi-Fi!");
}

#endif /* WIFI_INFO_H_ */
