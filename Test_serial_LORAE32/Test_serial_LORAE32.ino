//Comunicação através do LORA usando monitor serial para envio da localização geográfica e hora através de um struct
//LoRa E32 900MHz 
//GPS NEO-6M
//ESP32

//05/09/2024 -> nenhuma modificação só upload no git

#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Pin used in ESP32:
#define LORA_RX 16 //RX2
#define LORA_TX 17 //TX2

#define RX_GPS 13  // Pino RX do módulo GPS (conectado ao TX do GPS)
#define TX_GPS 12  // Pino TX do módulo GPS (conectado ao RX do GPS)
SoftwareSerial serialGPS(RX_GPS, TX_GPS);
TinyGPS gps;

SoftwareSerial LoRaSerial(LORA_RX, LORA_TX);

String URLmapa;

bool lido = false;

void leGPS();

void setup() {
  // Init serial communication
  Serial.begin(9600);
  LoRaSerial.begin(9600);

  serialGPS.begin(9600);
  Serial.println("GPS Iniciado!");

  Serial.println("ESP23 and LoRa E32 are Ready!");
}


void loop() {
    //Hora

  static unsigned long delayLeGPS = millis();

  if ((millis() - delayLeGPS) > 1000) {
    leGPS();
    delayLeGPS = millis();
  }
  String message = Serial.readStringUntil('\n');
  LoRaSerial.println(URLmapa);
  LoRaSerial.println(message);
  Serial.println("Send: " + URLmapa);
  delay(10);  

    // Read message from LoRa and display in Serial Monitor
  if (LoRaSerial.available()) {
    String message = LoRaSerial.readStringUntil('\n');
    Serial.println("Receive: " + message);
  }
  delay(10);
}

void leGPS() {
  unsigned long delayGPS = millis();
  unsigned long lastRead = millis();
  unsigned long idadeInfo;
  serialGPS.listen();
  int ano;
  uint8_t mes, dia, hora, minuto, segundo, centesimo;
  float altitudeGPS;
  altitudeGPS = gps.f_altitude(); //Pega a altitude do uBlox
  gps.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo); //Pega horário
  while ((millis() - delayGPS) < 500) {
    while (serialGPS.available()) {  
      char cIn = serialGPS.read();
      lido = gps.encode(cIn);
    }

    if (lido) {
      //Serial.println("leu");
      float flat, flon;
      unsigned long age;

      gps.f_get_position(&flat, &flon, &age);

      if (flat != TinyGPS::GPS_INVALID_F_ANGLE){
          Serial.println(float(flat / 100000));
      }
      if (flon != TinyGPS::GPS_INVALID_F_ANGLE){
          Serial.println(float(flon / 100000));
      }

      if ((altitudeGPS != TinyGPS::GPS_INVALID_ALTITUDE) && (altitudeGPS != 100000)) {
          Serial.print("Altitude (cm): ");
          Serial.println(altitudeGPS);
      }
      // urlMapa = "Local Identificado: https://maps.google.com/maps/?&z=10&q=";

      // Ajusta para GMT -03:00
      int localHour = hora - 3;
      
      // Lida com underflow (caso a hora seja < 0)
      if (localHour < 0) {
        localHour += 24;
        dia -= 1;  // Ajuste do dia
      }
      
      URLmapa = "";
      URLmapa += " - Time: ";
      URLmapa += String(localHour) + ":" + String(minuto) + ":" + String(segundo);
      URLmapa += ",";
      URLmapa += " Latitude: ";
      URLmapa += String(flat, 6);
      URLmapa += ",";
      URLmapa += " Longitude: ";
      URLmapa += String(flon, 6);
      URLmapa += " Altitude: ";
      URLmapa += String(altitudeGPS);
      break;
    }
  }
}
