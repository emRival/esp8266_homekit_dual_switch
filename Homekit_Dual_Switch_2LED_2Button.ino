#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"
#include <WiFiManager.h>

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// Pin HomeKit (relay/LED)
#define PIN_SWITCH1 16  // misal GPIO16
#define PIN_SWITCH2 5   // misal GPIO5

// Tombol fisik
#define BUTTON_PIN_1 13  // D7
#define BUTTON_PIN_2 15  // D8
#define LED_PIN_1    14  // D5
#define LED_PIN_2    12  // D6

#define BUILTIN_BUTTON 0  // Tombol flash di NodeMCU = GPIO0

unsigned long builtinButtonPressTime = 0;
bool builtinButtonLongPressed = false;
const unsigned long longPressDuration = 5000; // 5 detik


// State LED
bool ledState1 = false;
bool ledState2 = false;

// State tombol
bool lastButtonState1 = LOW;
bool lastButtonState2 = LOW;

unsigned long lastDebounceTime1 = 50;
unsigned long lastDebounceTime2 = 50;
const unsigned long debounceDelay = 100;

// HomeKit externs
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on1;
extern "C" homekit_characteristic_t cha_switch_on2;

// Setter HomeKit
void cha_switch_on_setter1(const homekit_value_t value) {
  bool on = value.bool_value;
  ledState1 = on;
  digitalWrite(LED_PIN_1, ledState1 ? HIGH : LOW);
  digitalWrite(PIN_SWITCH1, ledState1 ? LOW : HIGH);  // Relay biasanya aktif LOW
  LOG_D("HomeKit 1: %s", ledState1 ? "ON" : "OFF");
}

void cha_switch_on_setter2(const homekit_value_t value) {
  bool on = value.bool_value;
  ledState2 = on;
  digitalWrite(LED_PIN_2, ledState2 ? HIGH : LOW);
  digitalWrite(PIN_SWITCH2, ledState2 ? LOW : HIGH);
  LOG_D("HomeKit 2: %s", ledState2 ? "ON" : "OFF");
}

void my_homekit_setup() {
  pinMode(PIN_SWITCH1, OUTPUT);
  digitalWrite(PIN_SWITCH1, HIGH);
  pinMode(PIN_SWITCH2, OUTPUT);
  digitalWrite(PIN_SWITCH2, HIGH);

  cha_switch_on1.setter = cha_switch_on_setter1;
  cha_switch_on2.setter = cha_switch_on_setter2;

  arduino_homekit_setup(&config);
	// Kirim status LED 1 sebenarnya ke HomeKit
	cha_switch_on1.value.bool_value = ledState1;
	homekit_characteristic_notify(&cha_switch_on1, cha_switch_on1.value);

	// Kirim status LED 2 juga (kalau perlu)
	cha_switch_on2.value.bool_value = ledState2;
	homekit_characteristic_notify(&cha_switch_on2, cha_switch_on2.value);

}

void my_homekit_loop() {
  arduino_homekit_loop();
  static uint32_t next_heap_millis = 0;
  const uint32_t t = millis();
  if (t > next_heap_millis) {
    next_heap_millis = t + 5000;
    LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
  }
}

void setup() {
  Serial.begin(115200);
  wifi_connect();
  // homekit_storage_reset(); // Uncomment if needed
  my_homekit_setup();

  pinMode(BUTTON_PIN_1, INPUT);
  pinMode(BUTTON_PIN_2, INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);

  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_2, LOW);

	pinMode(BUILTIN_BUTTON, INPUT_PULLUP);

  LOG_D("Tombol toggle siap digunakan");
}

void loop() {
  my_homekit_loop();

  int reading1 = digitalRead(BUTTON_PIN_1);
  int reading2 = digitalRead(BUTTON_PIN_2);
  unsigned long now = millis();

  // Tombol 1
  if (reading1 == HIGH && lastButtonState1 == LOW && (now - lastDebounceTime1) > debounceDelay) {
    ledState1 = !ledState1;
    digitalWrite(LED_PIN_1, ledState1 ? HIGH : LOW);
    digitalWrite(PIN_SWITCH1, ledState1 ? LOW : HIGH);
    LOG_D("Tombol 1: %s", ledState1 ? "ON" : "OFF");
    cha_switch_on1.value.bool_value = ledState1;
    homekit_characteristic_notify(&cha_switch_on1, cha_switch_on1.value);
    lastDebounceTime1 = now;
  }
  lastButtonState1 = reading1;

  // Tombol 2
  if (reading2 == HIGH && lastButtonState2 == LOW && (now - lastDebounceTime2) > debounceDelay) {
    ledState2 = !ledState2;
    digitalWrite(LED_PIN_2, ledState2 ? HIGH : LOW);
    digitalWrite(PIN_SWITCH2, ledState2 ? LOW : HIGH);
    LOG_D("Tombol 2: %s", ledState2 ? "ON" : "OFF");
    cha_switch_on2.value.bool_value = ledState2;
    homekit_characteristic_notify(&cha_switch_on2, cha_switch_on2.value);
    lastDebounceTime2 = now;
  }
  lastButtonState2 = reading2;

	  // Tombol built-in NodeMCU (D3 / GPIO0) - Long Press Reset
  int builtinButtonState = digitalRead(BUILTIN_BUTTON);

  if (builtinButtonState == LOW && builtinButtonPressTime == 0) {
    builtinButtonPressTime = millis(); // Mulai hitung waktu tekan
  }

  if (builtinButtonState == LOW && !builtinButtonLongPressed && millis() - builtinButtonPressTime >= longPressDuration) {
    builtinButtonLongPressed = true;
    LOG_D("Tombol bawaan ditekan lama - RESET HOMEKIT");
    // Indikasi LED1 kedip
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN_1, HIGH);
      delay(200);
      digitalWrite(LED_PIN_1, LOW);
      delay(200);
    }
    homekit_storage_reset();

    WiFiManager wifiManager;
    wifiManager.resetSettings();
    ESP.reset();
    delay(500);
    ESP.restart();
  }

  if (builtinButtonState == HIGH) {
    builtinButtonPressTime = 0;
    builtinButtonLongPressed = false;
  }

}
