# 🔌 NodeMCU ESP8266 - Dual Switch HomeKit with WiFi Manager

Proyek ini menggunakan **NodeMCU ESP8266** untuk mengendalikan dua saklar (relay/LED) dengan integrasi **Apple HomeKit**, serta kontrol fisik menggunakan tombol. Proyek ini juga menyediakan **WiFi setup awal via Access Point (AP)** menggunakan **WiFiManager**.

---

## 🛠️ Spesifikasi Pin

| Fungsi             | GPIO | NodeMCU Pin |
|--------------------|------|-------------|
| HomeKit Switch 1   | 16   | D0          |
| HomeKit Switch 2   | 5    | D1          |
| Tombol Fisik 1     | 13   | D7          |
| Tombol Fisik 2     | 15   | D8          |
| LED Indikator 1    | 14   | D5          |
| LED Indikator 2    | 12   | D6          |
| Tombol Flash (built-in) | 0 | D3 (FLASH) |

---

## 📦 Library yang Digunakan

- [`WiFiManager` by tzapu](https://github.com/tzapu/WiFiManager) - **Versi: 2.0.16-rc.2**
- [`Arduino-HomeKit-ESP8266`](https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266) - **Versi: 1.2.0**

---

## 📶 Mode Setup Awal

Saat pertama kali dinyalakan (atau jika konfigurasi WiFi belum tersedia), perangkat akan membuat jaringan WiFi:

- **SSID:** `Rival_Dual_Switch_AP`
- **Akses IP:** [http://192.168.4.1](http://192.168.4.1)
- Di halaman ini, kamu bisa mengatur koneksi ke jaringan WiFi lokal.

---

## 🔁 Reset Konfigurasi

Untuk mereset konfigurasi WiFi dan HomeKit:

- **Tekan dan tahan tombol flash (GPIO 0)** selama **5 detik** saat perangkat menyala.
- Setelah itu, perangkat akan masuk kembali ke mode Access Point.

---

## 🍎 HomeKit Pairing

Setelah perangkat terhubung ke WiFi:

- Akan muncul sebagai **aksesoris HomeKit** di aplikasi *Home* pada perangkat Apple.
- Saat proses pairing, masukkan kode berikut: **111-11-111**


---

## 🔘 Kontrol Tombol Fisik

- Setiap tombol fisik dapat digunakan untuk mengaktifkan/menonaktifkan relay/LED yang sesuai.
- Status akan disinkronkan secara otomatis ke HomeKit.

---

## 💡 Catatan

- Pastikan Anda menggunakan versi library yang disebutkan agar kompatibilitas maksimal.
- Jika ingin mereset konfigurasi WiFi, tekan dan tahan tombol flash (GPIO 0) saat booting.

---

## 🧰 Platform & Tools

- **Board:** NodeMCU 1.0 (ESP-12E Module)
- **PlatformIO** atau **Arduino IDE**

---

Happy hacking! 🎉


