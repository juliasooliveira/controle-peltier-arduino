#include <OneWire.h>
#include <DallasTemperature.h>

// Pino digital onde o pino do meio (Sinal) do DS18B20 está conectado
const int pinoDadosDS18B20 = 5; 

// Configura as instâncias necessárias para a biblioteca
OneWire oneWire(pinoDadosDS18B20);
DallasTemperature sensors(&oneWire);

void setup() {
  // Inicializa a comunicação serial para podermos ver o resultado no computador
  Serial.begin(9600);
  Serial.println("--- Teste de Leitura Basica do DS18B20 ---");

  // Inicializa o sensor digital
  sensors.begin();
}

void loop() {
  // Envia o comando para o sensor fazer a medição física da temperatura
  sensors.requestTemperatures(); 
  
  // Busca o valor em graus Celsius do primeiro sensor encontrado na linha (Índice 0)
  float tempCelsius = sensors.getTempCByIndex(0); 
  
  // Verifica se o sensor está respondendo corretamente
  if (tempCelsius == DEVICE_DISCONNECTED_C) {
    Serial.println("Erro: Sensor DS18B20 nao encontrado! Verifique a fiacao e o resistor.");
  } else {
    // Exibe a temperatura atual no Monitor Serial
    Serial.print("Temperatura Atual: ");
    Serial.print(tempCelsius);
    Serial.println(" *C");
  }
  
  // Aguarda 1 segundo antes de fazer a próxima leitura
  delay(1000); 
}
