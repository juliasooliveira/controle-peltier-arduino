#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// INCLUSÃO DA BIBLIOTECA PID
#include <PID_v1.h>

// Configurações do Display OLED
#define LARGURA_TELA 128 
#define ALTURA_TELA 64 
#define OLED_RESET     -1 
Adafruit_SSD1306 display(LARGURA_TELA, ALTURA_TELA, &Wire, OLED_RESET);

// Definição dos Pinos
const int pinoDadosDS18B20 = 5; 
const int pinoMosfet = 10;       
const int btnMais = 2;
const int btnMenos = 3;

// Configuração do Sensor de Temperatura[cite: 3]
OneWire oneWire(pinoDadosDS18B20);
DallasTemperature sensors(&oneWire);

// VARIÁVEIS DO PID[cite: 3]
double tempAtual = 0.0;   // Entrada (Input) do PID
double potenciaPWM = 0.0; // Saída (Output) do PID -> Vai para o MOSFET
double tempAlvo = 16.0;   // Alvo (Setpoint) do PID

// CONSTANTES DE SINTONIA REAIS CALCULADAS (Valores em módulo/positivos)
double Kp = 94.2095;  // Ganho Proporcional Real
double Ki = 15.7016;  // Ganho Integral Real
double Kd = 141.3143; // Ganho Derivativo Real

// Inicializa o objeto PID no modo REVERSE para resfriamento
PID meuPID(&tempAtual, &potenciaPWM, &tempAlvo, Kp, Ki, Kd, REVERSE);

// Variáveis de controle dos botões (Debounce)[cite: 3]
unsigned long ultimoTempoBotao = 0;
const int delayDebounce = 200; 

void setup() {
  Serial.begin(9600);
  sensors.begin();

  pinMode(pinoMosfet, OUTPUT);
  pinMode(btnMais, INPUT_PULLUP);  
  pinMode(btnMenos, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar o Display OLED"));
    for(;;); 
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // CONFIGURAÇÃO INICIAL DO PID[cite: 1, 3]
  meuPID.SetMode(AUTOMATIC);          // Ativa o PID
  meuPID.SetOutputLimits(0, 255);     // Garante que o PID respeite os limites do PWM do Arduino
  meuPID.SetSampleTime(200);          // Faz o PID recalcular a cada 200 milissegundos
}

void loop() {
  // 1. Leitura da Temperatura Atual
  sensors.requestTemperatures();
  float tempLida = sensors.getTempCByIndex(0);
  
  if (tempLida != DEVICE_DISCONNECTED_C) {
    tempAtual = (double)tempLida; // O PID exige que seja do tipo 'double'
  }

  // 2. Leitura dos Botões para alterar a Temperatura Alvo
  if (millis() - ultimoTempoBotao > delayDebounce) {
    if (digitalRead(btnMais) == LOW) {
      tempAlvo += 1.0; 
      ultimoTempoBotao = millis();
    }
    if (digitalRead(btnMenos) == LOW) {
      tempAlvo -= 1.0; 
      ultimoTempoBotao = millis();
    }
  }
  tempAlvo = constrain(tempAlvo, 5.0, 25.0); // Limites de setpoint ajustados para resfriamento

  // 3. PROCESSAMENTO DO PID
  // Esta função analisa o cenário atual e recalcula o valor exato de 'potenciaPWM' automaticamente
  meuPID.Compute();

  // Envia o cálculo matemático do PID direto para o MOSFET
  analogWrite(pinoMosfet, (int)potenciaPWM);

  //4. Atualização do Display OLED
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("CONTROLADOR PID v1.1");

  display.setTextSize(1);
  display.setCursor(0, 16);
  display.print("ATUAL:");
  display.print(tempAtual, 1);
  display.print("C");

  Serial.print("tempAtual:");
  Serial.print(tempAtual);
  Serial.print(",tempAlvo:");
  Serial.print(tempAlvo);
  Serial.print(",PWM:");
  Serial.println(potenciaPWM);

  display.setTextSize(1);
  display.setCursor(0, 38);
  display.print("ALVO : ");
  display.print(tempAlvo, 1);
  display.print(" C");

  display.setCursor(0, 52);
  display.print("PWM: ");
  display.print(map((int)potenciaPWM, 0, 255, 0, 100));
  display.print("%");
  
  // Exibe os parâmetros atuais para monitoramento na bancada
  display.print("  P:");
  display.print((int)Kp);

  display.display();
  delay(100); 
}
