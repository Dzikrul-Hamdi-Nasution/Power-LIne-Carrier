#include <SPI.h>
#include <UIPEthernet.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>                                      // Memanggil library LCD i2C
LiquidCrystal_I2C lcd(0x27, 16, 2);



#define tombol_1 2
#define tombol_2 3
#define tombol_3 4
#define tombol_4 5

#define led_1 6
#define led_2 7
#define led_3 8
#define led_4 9
#define mode_sistem A0


const int serverPort = 80;
IPAddress server(192, 168, 100, 100);

byte mac[] = {0x90, 0xA2, 0xDA, 0x0D, 0x48, 0xD3 };
IPAddress ip(192, 168, 100, 177);
EthernetServer server_koneksi(serverPort);
EthernetClient TCPclient;
String dataIn;
String dt[5];
int i;
boolean parsing = false;

int state_button_1, state_button_2, state_button_3, state_button_4;
int t_1, t_2, t_3, t_4;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(mode_sistem, INPUT_PULLUP);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(tombol_1, INPUT_PULLUP);
  pinMode(tombol_2, INPUT_PULLUP);
  pinMode(tombol_3, INPUT_PULLUP);
  pinMode(tombol_4, INPUT_PULLUP);
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  digitalWrite(led_3, HIGH);
  digitalWrite(led_4, HIGH);

  Ethernet.begin(mac, ip);


  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

}

void loop() {



  if (digitalRead(mode_sistem) == LOW) {


    if (digitalRead(tombol_1) == LOW) {
      while (digitalRead(tombol_1) == LOW) {
      }
      if (state_button_1 == 0) {
        state_button_1 = 1;
      } else {
        state_button_1 = 0;
      }
    }

    if (digitalRead(tombol_2) == LOW) {
      while (digitalRead(tombol_2) == LOW) {
      }
      if (state_button_2 == 0) {
        state_button_2 = 1;
      } else {
        state_button_2 = 0;
      }
    }

    if (digitalRead(tombol_3) == LOW) {
      while (digitalRead(tombol_3) == LOW) {
      }
      if (state_button_3 == 0) {
        state_button_3 = 1;
      } else {
        state_button_3 = 0;
      }
    }

    if (digitalRead(tombol_4) == LOW) {
      while (digitalRead(tombol_4) == LOW) {
      }
      if (state_button_4 == 0) {
        state_button_4 = 1;
      } else {
        state_button_4 = 0;
      }
    }

    kontrol_button();

  } else {
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

}


void kontrol_button() {

  lcd.setCursor(0, 0);
  lcd.print("CONTROL MODE");


  if (state_button_1 == 1) {
    digitalWrite(led_1, LOW);
    t_1 = 1;
  } else {
    digitalWrite(led_1, HIGH);
    t_1 = 0;
  }

  if (state_button_2 == 1) {
    digitalWrite(led_2, LOW);
    t_2 = 1;
  } else {
    digitalWrite(led_2, HIGH);
    t_2 = 0;
  }

  if (state_button_3 == 1) {
    digitalWrite(led_3, LOW);
    t_3 = 1;
  } else {
    digitalWrite(led_3, HIGH);
    t_3 = 0;
  }

  if (state_button_4 == 1) {
    digitalWrite(led_4, LOW);
    t_4 = 1;
  } else {
    digitalWrite(led_4, HIGH);
    t_4 = 0;
  }

  if (TCPclient.connect(server, serverPort)) {
    Serial.println("Connected to TCP server");
    lcd.setCursor(0, 1);
    lcd.print("CONNECTED");
  } else {
    Serial.println("Failed to connect to TCP server");
    lcd.setCursor(0, 1);
    lcd.print("FAILED    ");
  }
  String total = "*" + String(t_1) + "," + String(t_2) + "," + String(t_3) + "," + String(t_4) + ",#";
  Serial.println(total);
  TCPclient.println(total);
  TCPclient.flush();

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

  Serial.println(dt[4]);
  Serial.println(dt[0]);
  Serial.println(dt[1]);
  Serial.println(dt[2]);
  Serial.println(dt[3]);

  lcd.setCursor(0, 0);
  lcd.print("I1=");
  lcd.print(dt[0]);

  lcd.setCursor(0, 1);
  lcd.print("I2=");
  lcd.print(dt[1]);

  lcd.setCursor(6, 0);
  lcd.print("I3=");
  lcd.print(dt[2]);

  lcd.setCursor(6, 1);
  lcd.print("I4=");
  lcd.print(dt[3]);

  lcd.setCursor(14, 0);
  lcd.print("V");
  lcd.setCursor(13, 1);
  lcd.print(dt[4]);

}
