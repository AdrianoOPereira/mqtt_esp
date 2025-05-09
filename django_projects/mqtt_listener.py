# COMANDO PARA RODAR O BROKER:
# C:\>cd mosquitto
# "C:\Program Files\mosquitto\mosquitto.exe" -c "C:\mosquitto\mosquitto.conf" -v

import paho.mqtt.client as mqtt


def on_connect(client, userdata, flags, rc):
    print(f"Conectado com código: {rc}")
    client.subscribe("sdkjgsdgryf45h3745/CAIXA/lampada1")

def on_message(client, userdata, msg):
    print(f"Recebido: {msg.topic} -> {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.6", 1883, 60)
client.loop_forever()
