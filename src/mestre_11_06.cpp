#include <WiFi.h> //mestre 19/06/2025
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Familia Bueno - EvolutionPro-2G";
const char* password = "asiv281006amv";
const char* mqttServer = "192.168.1.9";
const int mqttPort = 1883;

const char* topicoMestre = "painel/controle/mestre001";

WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length) {
  // Criação do buffer para leitura segura
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.println("Erro ao ler JSON. Verificando se é texto simples...");

    // Se não for JSON, tenta leitura simples como "espXX ON"
    String msg;
    for (int i = 0; i < length; i++) msg += (char)payload[i];
    msg.trim();

    Serial.printf("[Recebido texto simples] \"%s\"\n", msg.c_str());

    int sep = msg.indexOf(' ');
    if (sep == -1) {
      Serial.println("Formato inválido. Use: espXX COMANDO ou TODOS COMANDO");
      return;
    }

    String destino = msg.substring(0, sep);
    String comando = msg.substring(sep + 1);

    if (destino == "TODOS") {
      for (int i = 1; i <= 30; i++) {
        char topico[40];
        sprintf(topico, "caixa/esp%02d/acao", i);
        client.publish(topico, comando.c_str());
        Serial.printf("→ Publicado em [%s]: %s\n", topico, comando.c_str());
      }
    } else {
      char topico[40];
      sprintf(topico, "caixa/%s/acao", destino.c_str());
      client.publish(topico, comando.c_str());
      Serial.printf("→ Publicado em [%s]: %s\n", topico, comando.c_str());
    }

  } else {
    // JSON válido
    const char* caixa = doc["caixa"];
    const char* acao = doc["acao"];

    if (caixa && acao) {
      String topico = "caixa/" + String(caixa) + "/acao";
      client.publish(topico.c_str(), acao);
      Serial.printf("→ [JSON] Publicado em [%s]: %s\n", topico.c_str(), acao);
    } else {
      Serial.println("JSON incompleto: precisa de 'caixa' e 'acao'");
    }
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
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32-PONTE")) {
      Serial.println("Conectado ao broker!");
      client.subscribe(topicoMestre);
      Serial.printf("Inscrito no tópico: %s\n", topicoMestre);
    } else {
      Serial.print("Erro MQTT: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  conectarWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
