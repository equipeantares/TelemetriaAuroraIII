#include <SoftwareSerial.h>
#include <EBYTE.h>                

#define RX_LORA 3 // Pino RX do módulo LoRa
#define TX_LORA 2 // Pino TX do módulo LoRa

SoftwareSerial lora(RX_LORA, TX_LORA);

// Defina os pinos M0, M1 e AUX de acordo com a pinagem do ESP32
#define M0_LORA 11
#define M1_LORA 12
#define AUX_LORA 4

EBYTE LoRa(&lora, M0_LORA, M1_LORA, AUX_LORA);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  lora.begin(9600);
  LoRa.init();

  LoRa.Reset(); // Opcional: Reseta os parâmetros para os de fábrica

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
}
void loop() {
  
if (Serial.available()>0){
  String msg = Serial.readString();
  lora.print(msg);
  Serial.println(msg);
  
}
}