//Criar um mosquitto.conf personalizado, Crie uma pasta:
//C:\mosquitto\
//Crie o arquivo:
//C:\mosquitto\mosquitto.conf

//Cole o seguinte conteúdo:
//listener 1883
//allow_anonymous true

//Com as duas pasta criadas abaixo execute o comando no mcd para iniciar o borker:
//O caminho do mosquitto.exe (ex: C:\Program Files\mosquitto\mosquitto.exe)
//O caminho do mosquitto.conf (ex: C:\mosquitto\mosquitto.conf)
//"C:\Program Files\mosquitto\mosquitto.exe" -c "C:\mosquitto\mosquitto.conf" -v


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Familia Bueno - EvolutionPro-2G"; //usuario wifi da minha rede local
const char* password = "asiv281006amv"; //senha wifi da minha rede local
const char* mqttServer = "192.168.1.8"; //ip do seu computador
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  client.setServer(mqttServer, mqttPort);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Verifica se há entrada no Monitor Serial
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n'); // Lê até pressionar ENTER
    comando.trim(); // Remove espaços extras

    if (comando == "ON" || comando == "OFF") {
      Serial.print("Enviando comando MQTT: ");
      Serial.println(comando);
      client.publish("casa/lampada", comando.c_str());
    } else {
      Serial.println("Comando inválido! Digite ON ou OFF.");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP8266Master")) {
      Serial.println("Conectado ao broker!");
    } else {
      Serial.print("Falha na conexão. Estado: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
