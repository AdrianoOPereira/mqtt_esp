import paho.mqtt.client as mqtt
import time
from datetime import datetime
import json

broker = "192.168.1.9"
topico_presenca = "painel/presenca"
rotas = {}
ativos = {}  # idDispositivo → timestamp

def salvar_rotas():
    with open("rotas.json", "w") as f:
        json.dump(rotas, f, indent=2)

def carregar_rotas():
    global rotas
    try:
        with open("rotas.json") as f:
            rotas = json.load(f)
    except:
        rotas = {}

def exibir_dispositivos():
    agora = time.time()
    print("\n Dispositivos vistos nos últimos 2 minutos:")
    encontrados = False
    for esp, ts in ativos.items():
        if agora - ts < 120:
            print(f" - {esp} (último sinal: {datetime.fromtimestamp(ts).strftime('%H:%M:%S')})")
            encontrados = True
    if not encontrados:
        print(" Nenhum dispositivo ativo detectado.\n")

def on_message(client, userdata, msg):
    if msg.topic == topico_presenca:
        id_esp = msg.payload.decode().strip()
        ativos[id_esp] = time.time()
        print(f" Presença detectada: {id_esp}")

def associar_rota():
    exibir_dispositivos()
    esp = input("Digite o ID do ESP (ex: esp39fc3f): ").strip()
    apelido = input("Apelido para este ESP (ex: LUZ01): ").strip().upper()
    rotas[apelido] = esp
    salvar_rotas()
    print(f" Apelido associado: {apelido} → {esp}")

def listar_rotas():
    if not rotas:
        print(" Nenhuma rota salva.")
        return
    print("\n Rotas salvas:")
    for apelido, esp in rotas.items():
        print(f" - {apelido} → {esp}")

def main():
    carregar_rotas()
    client.connect(broker)
    client.loop_start()
    client.subscribe(topico_presenca)

    print("\n Painel de Controle - Modo Roteamento")
    while True:
        print("\nComandos disponíveis:")
        print(" 1 - Associar apelido a ESP")
        print(" 2 - Ver dispositivos conectados")
        print(" 3 - Listar rotas")
        print(" 4 - Sair")
        cmd = input("> ")

        if cmd == "1":
            associar_rota()
        elif cmd == "2":
            exibir_dispositivos()
        elif cmd == "3":
            listar_rotas()
        elif cmd == "4":
            break
        else:
            print("Comando inválido.")

client = mqtt.Client()
client.on_message = on_message

if __name__ == "__main__":
    main()
