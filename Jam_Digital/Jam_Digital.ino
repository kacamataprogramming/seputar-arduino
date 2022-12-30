#include <NTPClient.h> //Library NTP Client
#include <ESP8266WiFi.h> //Library Wifi untuk ESP8266
#include <WiFiUdp.h> //Library untuk protokol komunikasi UDP
#include <TM1637.h> //Library untuk modul segmen TM1637

String ssid     = "Test"; //Nama SSID WiFi kalian
String password = "12345678"; //Password WiFi kalian

#define CLK 12 // Pin CLK TM1637 pada D6
#define DIO 14 // Pin DIO TM1637 pada D5
TM1637 segmen(CLK, DIO); //Membuat objek segmen dari class TM1637

WiFiUDP ntpUDP;  //Membuat objek ntpUDP dari class WiFiUDP
NTPClient waktu(ntpUDP, 25200); //Membuat objek waktu dari class NTPClient untuk data waktunya

//NTPClient ini waktu defaultnya adalah UTC+0 atau GMT+0.
//Karena saya tinggal pada zona waktu GMT+7,
// sehingga kita berikan offset 25200 detik atau 7 jam setelah GMT+0.
//Offset ini kalian sesuaikan dimana kalian tinggal.
//Jika kalian tinggal pada zona waktu GMT+8, maka menghitung offsetnya adalah
//8 x 3600 detik = 28800

void setup() {
    segmen.begin(); //Memulai koneksi segmen
    WiFi.begin(ssid, password); //Menghubungkan ESP8266nya ke WiFi

    while ( WiFi.status() != WL_CONNECTED ) {
      delay(500);
      segmen.display("Off "); //Segmen akan menampilkan tulisan Off selama belum terhubung ke WiFi
    }
    segmen.clearScreen(); //Menghapus tulian pada segmen
    waktu.begin(); //Memulai koneksi ke server
}

void loop() {
    waktu.update(); //Mengupdate waktu dari server
    String menit=waktu.getMinutes() < 10 ? "0" + String(waktu.getMinutes()) : String(waktu.getMinutes());
    //Jika waktu menunjukkan kurang dari 10 menit,
    //maka kita tambahkan angka 0 di depan menitnya
    String jam=waktu.getHours() < 10 ? "0" + String(waktu.getHours()) : String(waktu.getHours());
    //Jika waktu menunjukkan kurang dari jam 10,
    //maka kita tambahkan angka 0 di depan jamya
    segmen.display(jam+menit); //Menampilkan jam dan menit
    segmen.switchColon(); //Membuat titik dua pada segmen nyala atau mati
    segmen.refresh(); //Me-refresh tampilan segmen
    delay(1000);
}
