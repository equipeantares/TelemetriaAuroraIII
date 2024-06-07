#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <EBYTE.h>

#define RX_LORA 3 // Pino RX do módulo LoRa
#define TX_LORA 2 // Pino TX do módulo LoRa

SoftwareSerial lora(RX_LORA, TX_LORA);

// Defina os pinos M0, M1 e AUX de acordo com a pinagem do ESP32
#define M0_LORA 11
#define M1_LORA 12
#define AUX_LORA 4

EBYTE LoRa(&lora, M0_LORA, M1_LORA, AUX_LORA);

SoftwareSerial serialGPS(6, 7);

TinyGPS gps; 

String urlMapa;

bool lido = false;

void leGPS();

void setup() {

  Serial.begin(9600);
  Serial.println("Sketch Iniciado!");

  serialGPS.begin(9600);
  
  //while (!Serial);

  lora.begin(9600);
  LoRa.init();

  // LoRa.Reset(); // Opcional: Reseta os parâmetros para os de fábrica

  // Defina a taxa de dados de acordo com suas necessidades
  LoRa.SetAirDataRate(ADR_1K);

  // Defina o endereço da rede
  LoRa.SetAddress(1);

  // Defina o canal de comunicação
  LoRa.SetChannel(23);

  // Salve as configurações
  LoRa.SaveParameters(TEMPORARY);

  // Imprima os parâmetros do módulo
  LoRa.PrintParameters();

  // Defina o modo de operação
  LoRa.SetMode(MODE_NORMAL);

  leGPS();
}

void loop() {
  static unsigned long delayLeGPS = millis();

  if ((millis() - delayLeGPS) > 10000) {
    leGPS();
    
    delayLeGPS = millis();
  }

  if (lido){
  //  String msg = urlMapa;
    lora.print(urlMapa);
    Serial.println(urlMapa);
    lido = false;
  }
  

}

void leGPS() {
  unsigned long delayGPS = millis();
  unsigned long lastRead = millis();

  serialGPS.listen();
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

      // urlMapa = "Local Identificado: https://maps.google.com/maps/?&z=10&q=";
      urlMapa = "";
      urlMapa += String(flat, 6);
      urlMapa += ",";
      urlMapa += String(flon, 6);
      Serial.println(urlMapa);
      //lora.listen();
      //lora.print(urlMapa);

      break;
    }
  }
}

