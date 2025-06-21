import paho.mqtt.publish as publish
import json

broker = "192.168.1.9"
topico_mestre = "painel/controle/mestre001"
publish.single("caixa/esp000000/acao", "RESET", hostname="192.168.1.9")

# Cadastro de rotas: apelido → destino real
##rotas = {
    ##"TZO": "esp39fc3f ON",    
    ##"TZX": "esp123456 ON", 
    ##"OFF1": "esp39fc3f OFF",
    ##"OFF2": "esp123456 OFF",
##}

rotas = {
    "TZO": [
        {"caixa": "esp3afc3f", "acao": "ON"}
    ],
    "SSA": [
        {"caixa": "esp50cb02", "acao": "ON"}
    ],
    "OFF1": [
        {"caixa": "esp39fc3f", "acao": "OFF"}
    ],
    "OFF2": [
        {"caixa": "esp50cb02", "acao": "OFF"}
    ],
    "GRUPO1": [
        {"caixa": "esp39fc3f", "acao": "ON"},
        {"caixa": "esp50cb02", "acao": "ON"}
    ]
}



def main():
    print("Digite o nome da rota (ex: TZO, TZX, GRUPO1):")
    while True:
        entrada = input("> ").strip().upper()

        if entrada in rotas:
            comandos = rotas[entrada]
            for cmd in comandos:
                msg_json = json.dumps(cmd)
                publish.single(topico_mestre, msg_json, hostname=broker)
                print(f"Publicado: {msg_json}")
        else:
            print("Rota não cadastrada. Verifique o nome ou cadastre em 'rotas'.")


if __name__ == "__main__":
    main()




