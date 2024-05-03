
/*
    Step counter for BTT Octopus 

    Developer: Mauricio Neves Junior
    Date: 2024/05/02
    Version: v1.00

*/

/* LIBRARIES AND DEFINITIONS */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

/* VARIABLES */
const char* ssid = "Mauricio_2.4G";
const char* password = "MNJR8830@#@#";
const char* mqtt_server = "broker.hivemq.com";

const int ledPin = 2;
const int ESP_GPIO = 4; // GPIO 4
const char* topic = "topic/step";

WiFiClient espClient;
PubSubClient client(espClient);


/* FUNCTIONS */
//Function to connect to Wifi network
void wifi_connect(){
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado à rede WiFi");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
}    

//Callback to Recieve the Message from ESP8266
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//Function to check the connection status
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT Broker...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado");
      client.subscribe("topic/step");
    } else {
      Serial.print("Falha na conexão, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

//Global Setup
void setup(){
    wifi_connect();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(ESP_GPIO, INPUT);
}

//Global Loop
void loop(){
  if (!client.connected()) {
    reconnect();
  }
  
   // Read GPIO pin state
  int gpioState = digitalRead(ESP_GPIO);
  Serial.println(gpioState);
    // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle < 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(1);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle > 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledPin, dutyCycle);
    delay(1);
  }
  // Publish GPIO pin state to MQTT topic
  char message[2];
  message[0] = gpioState;
  message[1] = '\0';
  client.publish(topic, message);
  
  delay(1000); // Delay for stability
}