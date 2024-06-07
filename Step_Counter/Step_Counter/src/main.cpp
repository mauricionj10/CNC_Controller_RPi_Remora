
/*
    Step counter for BTT Octopus 

    Developer: Mauricio Neves Junior
    Date: 2024/05/02
    Version: v1.05

*/

/* LIBRARIES AND DEFINITIONS */
#include <Arduino.h>

/* VARIABLES */
// Defina os pinos conectados aos pinos "step" dos drivers dos motores de passo
const int pulsePinX = D2; // Pino para o eixo X
const int pulsePinY = D5; // Pino para o eixo Y
const int pulsePinZ = D1; // Pino para o eixo Z
const int led = D8;

// Variáveis para contar os pulsos
volatile unsigned long pulseCountX = 0;
volatile unsigned long pulseCountY = 0;
volatile unsigned long pulseCountZ = 0;

/* FUNCTIONS */
// Funções de interrupção na borda de subida
void IRAM_ATTR handlePulseRisingX() {
  pulseCountX++;
}

void IRAM_ATTR handlePulseRisingY() {
  pulseCountY++;
}

void IRAM_ATTR handlePulseRisingZ() {
  pulseCountZ++;
}

// Funções de interrupção na borda de descida
void IRAM_ATTR handlePulseFallingX() {
  pulseCountX = 0;
}

void IRAM_ATTR handlePulseFallingY() {
  pulseCountY = 0;
}

void IRAM_ATTR handlePulseFallingZ() {
  pulseCountZ = 0;
}

void setup() {
  // Inicializa a serial para monitoramento
  Serial.begin(115200);

  // Configura os pinos como entrada
  pinMode(pulsePinX, INPUT);
  pinMode(pulsePinY, INPUT);
  pinMode(pulsePinZ, INPUT);
  pinMode(led, OUTPUT);

  // Anexa as funções de interrupção aos pinos
  attachInterrupt(digitalPinToInterrupt(pulsePinX), handlePulseRisingX, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePinY), handlePulseRisingY, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePinZ), handlePulseRisingZ, RISING);
  // Imprime mensagem inicial
  Serial.println("Contador de Pulsos Iniciado");
}

void loop() {
  // Exibe o valor do contador de pulsos a cada segundo
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();
  digitalWrite(led, HIGH);

  if (currentTime - lastPrintTime >= 1000) {
    Serial.print("Contagem de Pulsos X: ");
    Serial.println(pulseCountX);
    Serial.print("Contagem de Pulsos Y: ");
    Serial.println(pulseCountY);
    Serial.print("Contagem de Pulsos Z: ");
    Serial.println(pulseCountZ);
    Serial.println("\n");
    lastPrintTime = currentTime;
  }
}