#include <ESP8266WiFi.h>          // escravo 19/06/2025
#include <PubSubClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

const char* ssid = "Familia Bueno - EvolutionPro-2G";
const char* password = "asiv281006amv";
const char* mqttServer = "192.168.1.9";
const int mqttPort = 1883;

#define EEPROM_SIZE 64
#define LED_PIN 2
#define RESET_PIN 4

WiFiClient espClient;
PubSubClient client(espClient);

String idDispositivo;
String apelido = "";

char topicoComando1[40];  // caixa/<id>/acao
char topicoComando2[40];  // caixa/<apelido>/acao

const char* TOPICO_NOVOS = "painel/registro/novos";
const char* TOPICO_ASSOCIAR = "painel/registro/associar";
const char* TOPICO_CONFIRMADO = "painel/registro/confirmado";
const char* TOPICO_PRESENCA = "painel/presenca";

unsigned long ultimaPresenca = 0;

void piscarLED(int vezes = 3) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

void carregarID() {
  EEPROM.begin(EEPROM_SIZE);
  char buffer[16];
  for (int i = 0; i < 16; i++) buffer[i] = EEPROM.read(i);
  buffer[15] = '\0';
  idDispositivo = String(buffer);
  idDispositivo.trim();

  if (idDispositivo == "" || idDispositivo.startsWith("clear")) {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char novoID[16];
    sprintf(novoID, "esp%02X%02X%02X", mac[3], mac[4], mac[5]);
    idDispositivo = String(novoID);
    idDispositivo.toLowerCase();
    for (int i = 0; i < idDispositivo.length(); i++) {
      EEPROM.write(i, idDispositivo[i]);
    }
    EEPROM.write(idDispositivo.length(), '\0');
    EEPROM.commit();
  }
  Serial.println("ID carregado: " + idDispositivo);
}

void carregarApelido() {
  char buf[20];
  for (int i = 0; i < 20; i++) buf[i] = EEPROM.read(20 + i);
  buf[19] = '\0';
  apelido = String(buf);
  apelido.trim();
  if (apelido != "") Serial.println("Apelido carregado: " + apelido);
}

void registrarDispositivo() {
  DynamicJsonDocument doc(128);
  doc["id"] = idDispositivo;
  doc["estado"] = "NOVO_ID";
  char buffer[128];
  serializeJson(doc, buffer);
  client.publish(TOPICO_NOVOS, buffer);
  Serial.println("Publicado pedido de associação.");
}

void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (!error) {
    const char* recebidoID = doc["id"];
    const char* recebidoApelido = doc["apelido"];

    if (String(recebidoID) == idDispositivo && recebidoApelido) {
      apelido = String(recebidoApelido);
      for (int i = 0; i < apelido.length(); i++) {
        EEPROM.write(20 + i, apelido[i]);
      }
      EEPROM.write(20 + apelido.length(), '\0');
      EEPROM.commit();
      Serial.println("Apelido associado: " + apelido);
      piscarLED();  // Confirma com LED

      // Confirma via MQTT
      DynamicJsonDocument conf(128);
      conf["apelido"] = apelido;
      conf["id"] = idDispositivo;
      conf["status"] = "ASSOCIADO";
      char buffer[128];
      serializeJson(conf, buffer);
      client.publish(TOPICO_CONFIRMADO, buffer);
    }
    return;
  }

  // Se não for JSON: comando comum
  String msg;
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();

  Serial.printf("[Comando recebido] %s\n", msg.c_str());

  if (msg == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ligado");
  } else if (msg == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED desligado");
  } else if (msg == "RESET") {
    for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0);
    EEPROM.write(0, 'c'); EEPROM.write(1, 'l'); EEPROM.write(2, 'e');
    EEPROM.write(3, 'a'); EEPROM.write(4, 'r'); EEPROM.write(5, '\0');
    EEPROM.commit();
    Serial.println("EEPROM limpa. Reinicie o ESP.");
  }
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi conectado.");
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando ao MQTT...");
    if (client.connect(idDispositivo.c_str())) {
      Serial.println("MQTT conectado!");

      client.subscribe(TOPICO_ASSOCIAR);

      sprintf(topicoComando1, "caixa/%s/acao", idDispositivo.c_str());
      client.subscribe(topicoComando1);

      if (apelido != "") {
        sprintf(topicoComando2, "caixa/%s/acao", apelido.c_str());
        client.subscribe(topicoComando2);
      }
    } else {
      Serial.println("Tentando novamente em 2s...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  EEPROM.begin(EEPROM_SIZE);

  if (digitalRead(RESET_PIN) == LOW) {
    for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0);
    EEPROM.write(0, 'c'); EEPROM.write(1, 'l'); EEPROM.write(2, 'e');
    EEPROM.write(3, 'a'); EEPROM.write(4, 'r'); EEPROM.write(5, '\0');
    EEPROM.commit();
    Serial.println("EEPROM limpa. Reinicie o ESP.");
    while (true);
  }

  carregarID();
  carregarApelido();
  conectarWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  registrarDispositivo();  // Envia JSON pedindo associação
  client.publish(TOPICO_PRESENCA, idDispositivo.c_str());
  ultimaPresenca = millis();
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  if (millis() - ultimaPresenca > 30000) {
    client.publish(TOPICO_PRESENCA, idDispositivo.c_str());
    ultimaPresenca = millis();
  }
}
