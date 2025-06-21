#include <ESP8266WiFi.h> // Biblioteca C++ específica do ESP8266 para conexão Wi-Fi
#include <PubSubClient.h> // Biblioteca C++ para comunicação com broker MQTT (como Mosquitto)

const char* ssid = "Familia Bueno - EvolutionPro-2G"; //usuario wifi da minha rede local
const char* password = "asiv281006amv"; //senha wifi da minha rede local
const char* mqttServer = "192.168.1.9"; //ip do seu computador
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200); //Inicializa a comunicação serial com o monitor serial a 115200 bauds (velocidade de transmissão de dados)
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
    reconnect(); //Define o IP e porta do broker MQTT e chama a função reconnect() para conectar
  }
  client.loop(); //client.loop() processa mensagens recebidas e mantém o cliente vivo

  // Verifica se há entrada no Monitor Serial
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n'); // Lê até pressionar ENTER
    comando.trim(); // Remove espaços extras

    if (comando == "TZO") {
      client.publish("sdkjgsdgryf45h3745/CAIXA/lampada1", comando.c_str());
      Serial.println("Liga lampada tzo");
    } else if (comando == "OFF1") {
      client.publish("sdkjgsdgryf45h3745/CAIXA/lampada1", comando.c_str());
      Serial.println("Desliga lampada tzo");
    } else if (comando == "TZN") {
      client.publish("sdkjgsdgryf45h3746/CAIXA/lampada2", comando.c_str());
      Serial.println("Liga lampada tzn");
    } else if (comando == "OFF2") {
      client.publish("sdkjgsdgryf45h3746/CAIXA/lampada2", comando.c_str());
      Serial.println("Desliga lampada tzn");
    } else {
      Serial.println("Rota não cadastrada, favor procurar o time do TI !!!");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP8266Master")) {
      Serial.println("Broker conectado!!!");
    } else {
      Serial.print("Falha na conexão.");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
