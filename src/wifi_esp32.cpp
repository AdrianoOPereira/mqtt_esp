#include "WiFi.h"
//Bloco de configuração WIFI
const char* ssid = "Familia Bueno - EvolutionPro-2G";
const char* password =  "asiv281006amv";

WiFiServer wifiServer(80);

void setup() {

  Serial.begin(115200);

  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a rede WIFI..");
  }

  Serial.println("Conectado a REDE WIFI ");
  Serial.println(WiFi.localIP());

  wifiServer.begin();
}

void loop() {

  WiFiClient client = wifiServer.available();

  if (client) {

    Serial.print("Cliente conectado com IP:");
    Serial.println(client.remoteIP());

    client.stop();

  }

delay(100);
}