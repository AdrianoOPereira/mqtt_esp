from django.shortcuts import render, redirect
from django.http import JsonResponse
from core.serial import arduino, comandos_validos
from django.contrib.auth.decorators import login_required
from  .forms import AwbForms
import serial

@login_required(login_url='/login')
def index(request):    
    return render(request, 'index.html')

@login_required(login_url='/login/')
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


