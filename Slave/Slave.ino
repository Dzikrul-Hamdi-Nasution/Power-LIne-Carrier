#include <SPI.h>
#include <UIPEthernet.h>
#include "ZMPT101B.h"
#include "ACS712.h"


ZMPT101B voltageSensor(A4);
ACS712 ACS_1(A0, 5.0, 1023, 185);
ACS712 ACS_2(A1, 5.0, 1023, 185);
ACS712 ACS_3(A2, 5.0, 1023, 185);
ACS712 ACS_4(A3, 5.0, 1023, 185);
byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x48, 0xD4 };

// The IP address will be dependent on your local network:
// assign an IP address for the controller:
IPAddress ip(192, 168, 100, 100);
IPAddress server(192, 168, 100, 177);
const int serverPort = 80;

// Initialize the Ethernet server library with the port you want to use.

#define relay_1 2
#define relay_2 3
#define relay_3 4
#define relay_4 5
#define mode_sistem 6

float suhu, daya_kwh;
int sensorValue_sensor_tegangan = 0, tegangan_PLN;
int mA_1, mA_2, mA_3, mA_4;
String dataIn;
String dt[5];
int i;
boolean parsing = false;
int kunci = 1;

String readString;

EthernetServer server_koneksi(serverPort);

EthernetClient TCPclient;

void setup()
{
  Serial.begin(9600);
  //start Ethernet
  Ethernet.begin(mac, ip);

  Serial.print("client is at ");
  Serial.println(Ethernet.localIP());
  voltageSensor.calibrate();
  ACS_1.autoMidPoint();
  ACS_2.autoMidPoint();
  ACS_3.autoMidPoint();
  ACS_4.autoMidPoint();

  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  pinMode(mode_sistem, INPUT_PULLUP);

  digitalWrite(relay_1, HIGH);
  digitalWrite(relay_2, HIGH);
  digitalWrite(relay_3, HIGH);
  digitalWrite(relay_4, HIGH);



}

void loop() {

  if (digitalRead(mode_sistem) == LOW) {
    server_koneksi.begin();
    cek_tombol();
  } else {
    if (TCPclient.connect(server, serverPort))
      Serial.println("Connected to TCP server");
    else
      Serial.println("Failed to connect to TCP server");

    digitalWrite(relay_1, LOW);
    digitalWrite(relay_2, LOW);
    digitalWrite(relay_3, LOW);
    digitalWrite(relay_4, LOW);

    float U = voltageSensor.getVoltageAC();
    float regresi_tegangan = U * 10;
    tegangan_PLN = regresi_tegangan - 30;
    if (tegangan_PLN < 30) {
      tegangan_PLN = 0;
    }

    mA_1 = ACS_1.mA_AC();
    mA_2 = ACS_2.mA_AC();
    mA_3 = ACS_3.mA_AC();
    mA_4 = ACS_4.mA_AC();


    String total = "*" + String(mA_1) + "," + String(mA_2) + "," + String(mA_3) + "," + String(mA_4) + "," + String(tegangan_PLN) + ",#";
    Serial.println(total);
    TCPclient.println(total);
    TCPclient.flush();
    delay(500);
  }

}


void cek_tombol() {
  server_koneksi.begin();
  EthernetClient client = server_koneksi.available();

  if (client) {
    char inChar = client.read();
    dataIn += inChar;
    if (inChar == '\n') {
      parsing = true;
    }
    if (parsing) {
      parsingData();
      parsing = false;
      dataIn = "";
    }
  }
}



void parsingData() {
  int j = 0;
  //kirim data yang telah diterima sebelumnya

  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 1; i < dataIn.length(); i++) {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ',')) {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = "";  //inisialisasi variabel array dt[j]
    } else {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
    }
  }


  Serial.println(dt[0]);
  Serial.println(dt[1]);
  Serial.println(dt[2]);
  Serial.println(dt[3]);

  if(dt[0]=="1"){
    digitalWrite(relay_1, LOW);
  }else{
    digitalWrite(relay_1, HIGH);
  }
  if(dt[1]=="1"){
    digitalWrite(relay_2, LOW);
  }else{
    digitalWrite(relay_2, HIGH);
  }
  if(dt[2]=="1"){
    digitalWrite(relay_3, LOW);
  }else{
    digitalWrite(relay_3, HIGH);
  }
  if(dt[3]=="1"){
    digitalWrite(relay_4, LOW);
  }else{
    digitalWrite(relay_4, HIGH);
  }

  
 


}
