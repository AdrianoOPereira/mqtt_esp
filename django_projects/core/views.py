from django.shortcuts import render
from django.http import JsonResponse
from core.src.test import conectar_arduino
from core.serial import arduino, comandos_validos
import serial


def index(request):
    return render(request, "index.html")


def enviar_comando_arduino(request, cmd):
    if cmd not in comandos_validos:
        return JsonResponse({'mensagem': 'Rota não cadastrada, procurar o time do TI...'}, status=400)
        
    if not arduino or not arduino.is_open:
        return JsonResponse({'mensagem': 'Erro: Arduino não está conectado'}, status=500)

    try:
        arduino.write(cmd.encode())
        arduino.flush()
        return JsonResponse({'mensagem': f'Comando {cmd} enviado com sucesso!'})
    except Exception as e:
        return JsonResponse({'mensagem': f'Erro ao enviar comando: {str(e)}'}, status=500)
        
        
        