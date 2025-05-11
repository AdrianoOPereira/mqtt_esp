#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Familia Bueno - EvolutionPro-2G";
const char* password = "asiv281006amv";
const char* mqttServer = "192.168.1.6";
const int mqttPort = 1883;
const int lampadaPin = 2;  // Pino da lâmpada (GPIO2)

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim(); // Remove espaços extras

  Serial.print("Comando recebido: ");
  Serial.println(msg);

  if (msg == "ON") {
    digitalWrite(lampadaPin, HIGH);
    Serial.println("Lâmpada ACESA!");
  } else if (msg == "OFF") {
    digitalWrite(lampadaPin, LOW);
    Serial.println("Lâmpada APAGADA!");
  } else {
    Serial.println("Comando inválido!");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(lampadaPin, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnect();
  client.subscribe("casa/lampada");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Slave")) {
      Serial.println("Conectado ao broker!");
      client.subscribe("casa/lampada");
    } else {
      Serial.print("Falha na conexão. Estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

