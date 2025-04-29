#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Familia Bueno - EvolutionPro-2G";
const char* password = "asiv281006amv";
const char* mqttServer = "192.168.1.8";
const int mqttPort = 1883;
const int lampadaPin = 2;  // Pino da lâmpada (GPIO2)

const char* TOPICO_LAMPADA = "sdkjgsdgryf45h3745/CAIXA/lampada1";
const char* CLIENT_ID = "esp32slave_1";

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

  if (msg == "TZO") {
    digitalWrite(lampadaPin, HIGH);
    Serial.println("Lâmpada ACESA!");
  } else if (msg == "OFF1") {
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
    if (client.connect("esp32slave_1")) {
      Serial.println("Conectado ao broker!");
      client.subscribe("sdkjgsdgryf45h3745/CAIXA/lampada1");
    } else {
      Serial.print("Falha na conexão. Estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
