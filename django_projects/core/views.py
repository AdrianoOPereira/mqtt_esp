from django.shortcuts import render
from django.http import JsonResponse
from core.src.test import conectar_arduino
import paho.mqtt.publish as publish
import serial
#from django.http import JsonResponse

def index(request):
    return render(request, "index.html")

comandos_validos = ['TZO', 'OFF1', 'TZN', 'OFF2']

# Tente abrir a conexão apenas uma vez
try:
    arduino = serial.Serial("COM3", baudrate=115200, timeout=1)
    print("Arduino conectado!!!")
except Exception as e:
    print(f"Erro ao conectar Arduino: {e}")
    arduino = None

def enviar_comando_arduino(request, cmd):
    if cmd not in comandos_validos:
        return JsonResponse({'mensagem': 'Comando inválido'}, status=400)

    if not arduino or not arduino.is_open:
        return JsonResponse({'mensagem': 'Erro: Arduino não está conectado'}, status=500)

    try:
        arduino.write(cmd.encode())
        arduino.flush()
        return JsonResponse({'mensagem': f'Comando {cmd} enviado com sucesso!'})
    except Exception as e:
        return JsonResponse({'mensagem': f'Erro ao enviar comando: {str(e)}'}, status=500)
