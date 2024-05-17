
/*
    Step counter for BTT Octopus 

    Developer: Mauricio Neves Junior
    Date: 2024/05/02
    Version: v1.00

*/

/* LIBRARIES AND DEFINITIONS */
#include <Arduino.h>


/* VARIABLES */
// Defina o pino conectado ao pino "step" do driver do motor de passo
const int pulsePin = D2;

// Variável para contar os pulsos
volatile unsigned long pulseCount = 0;

/* FUNCTIONS */
// Função de interrupção
void IRAM_ATTR handlePulse() {
  pulseCount++;
}

void setup() {
  // Inicializa a serial para monitoramento
  Serial.begin(115200);

  // Configura o pino como entrada
  pinMode(pulsePin, INPUT);

  // Anexa a função de interrupção ao pino, na borda de subida (RISING)
  attachInterrupt(digitalPinToInterrupt(pulsePin), handlePulse, RISING);

  // Imprime mensagem inicial
  Serial.println("Contador de Pulsos Iniciado");
}

void loop() {
  // Exibe o valor do contador de pulsos a cada segundo
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastPrintTime >= 1000) {
    Serial.print("Contagem de Pulsos: ");
    Serial.println(pulseCount);
    lastPrintTime = currentTime;
  }
}
