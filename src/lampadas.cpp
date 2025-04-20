void setup() {
    Serial.begin(9600);
    Serial.setTimeout(50);
  
    // Definir pinos de 1 a 10 como saída
    for (int i = 0; i <= 10; i++) {
      pinMode(i, OUTPUT);
      digitalWrite(i, LOW);
    }
  }
  
  void loop() {
  
    if (Serial.available() > 0) {
      String recebido = Serial.readStringUntil('\n');
      recebido.trim(); // Remove espaços em branco e quebras de linha
  
      //if (recebido.startsWith("0")) {
        //recebido.remove(0,1); // Remove o primeiro caractere "0"
        
        int pino = recebido.toInt(); // Converte para inteiro
        
        if (pino >= 0 && pino <= 10) { // Garante que o pino está no intervalo válido
          digitalWrite(pino, HIGH);
          //Serial.println(pino, HIGH );
          delay(1000);
          digitalWrite(pino, LOW);
          delay(10);
        }
      }
    //}
  }
  