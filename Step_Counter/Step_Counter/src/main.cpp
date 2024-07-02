
/*
    Step counter for BTT Octopus 

    Developer: Mauricio Neves Junior
    Date: 2024/05/02
    Version: v1.15

*/

/* LIBRARIES AND DEFINITIONS */
#include <Arduino.h>

/* VARIABLES */
// Defina os pinos conectados aos pinos "step" dos drivers dos motores de passo
const int pulsePinX = D1; // Pino para o eixo X
const int pulsePinY = D2; // Pino para o eixo Y
const int pulsePinZ = D5; // Pino para o eixo Z
const int dirX = D6;
const int dirY = D7;
const int dirZ = D8;

const int led = D4;

// Variáveis para contar os pulsos
volatile unsigned long pulseCountX = 0;
volatile unsigned long pulseCountY = 0;
volatile unsigned long pulseCountZ = 0;
volatile unsigned long lastPulseCountX = 0;
volatile unsigned long lastPulseCountY = 0;
volatile unsigned long lastPulseCountZ = 0;
float frequencyX = 0.0;
float frequencyY = 0.0;
float frequencyZ = 0.0;

// Variáveis para armazenar os estados dos pinos de direção
bool currentDirX = LOW;
bool currentDirY = LOW;
bool currentDirZ = LOW;
bool lastDirX = LOW;
bool lastDirY = LOW;
bool lastDirZ = LOW;

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
  pinMode(dirX, INPUT);
  pinMode(dirY, INPUT);
  pinMode(dirZ, INPUT);
  pinMode(led, OUTPUT);

  // Anexa as funções de interrupção aos pinos
  attachInterrupt(digitalPinToInterrupt(pulsePinX), handlePulseRisingX, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePinY), handlePulseRisingY, RISING);
  attachInterrupt(digitalPinToInterrupt(pulsePinZ), handlePulseRisingZ, RISING);
  // Imprime mensagem inicial
  Serial.println("Contador de Pulsos Iniciado");
}

void loop() {
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();

  // Calcular a frequência a cada segundo
  if (currentTime - lastPrintTime >= 1000) {
    // Calcular a frequência para o eixo X
    frequencyX = (pulseCountX - lastPulseCountX) / 1.0; // Pulsos por segundo
    lastPulseCountX = pulseCountX;

    // Calcular a frequência para o eixo Y
    frequencyY = (pulseCountY - lastPulseCountY) / 1.0; // Pulsos por segundo
    lastPulseCountY = pulseCountY;

    // Calcular a frequência para o eixo Z
    frequencyZ = (pulseCountZ - lastPulseCountZ) / 1.0; // Pulsos por segundo
    lastPulseCountZ = pulseCountZ;

    // Imprimir os valores dos contadores de pulsos e as frequências
    Serial.print("Eixo X: ");
    Serial.println(pulseCountX);
    Serial.print("Frequencia X: ");
    Serial.print(frequencyX);
    Serial.println(" Hz");

    Serial.print("Direcao X: ");
    if (pulseCountX == 0) {
      Serial.println("Parado");
    } else if (currentDirX != lastDirX) {
      Serial.println(currentDirX ? "Direita" : "Esquerda");
      lastDirX = currentDirX;
    }

    Serial.print("\nEixo Y: ");
    Serial.println(pulseCountY);
    Serial.print("Frequencia Y: ");
    Serial.print(frequencyY);
    Serial.println(" Hz");

    Serial.print("Direcao Y: ");
    if (pulseCountY == 0) {
      Serial.println("Parado");
    } else if (currentDirY != lastDirY) {
      Serial.println(currentDirY ? "Direita" : "Esquerda");
      lastDirY = currentDirY;
    }

    Serial.print("\nEixo Z: ");
    Serial.println(pulseCountZ);
    Serial.print("Frequencia Z: ");
    Serial.print(frequencyZ);
    Serial.println(" Hz");

    Serial.print("Direcao Z: ");
    if (pulseCountZ == 0) {
      Serial.println("Parado");
    } else if (currentDirZ != lastDirZ) {
      Serial.println(currentDirZ ? "Direita" : "Esquerda");
      lastDirZ = currentDirZ;
    }

    Serial.println("\n");
    lastPrintTime = currentTime;
  }
}