from django.shortcuts import render
from django.http import JsonResponse
# from core.src.test import conectar_arduino
from core.serial import arduino, comandos_validos
import serial


def index(request):
    return render(request, "index.html") # render = para retornar páginas HTML.


def enviar_comando_arduino(request, cmd):
    if cmd not in comandos_validos:
    # Verifica se o comando recebido está presente na lista comandos_validos.
    # Se não estiver, retorna erro 400 com uma mensagem personalizada.
    # arduino, comandos_validos: está importando um objeto arduino e uma lista de comandos válidos de um módulo chamado core.serial.
        return JsonResponse({'mensagem': 'Rota não cadastrada, procurar o time do TI...'}, status=400)
    # JsonResponse: para retornar mensagens em formato JSON (muito útil para APIs e frontend que consome dados via JavaScript).
        
    if not arduino or not arduino.is_open:
        # Se o Arduino não estiver conectado ou a porta estiver fechada, retorna erro 500.
        return JsonResponse({'mensagem': 'Erro: Arduino não está conectado'}, status=500)

    try:
        arduino.write(cmd.encode())
        # Envia o comando codificado em bytes via serial.write.
        arduino.flush()
        # Usa flush() para garantir que o comando foi enviado imediatamente.
        return JsonResponse({'mensagem': f'Comando {cmd} enviado com sucesso!'})
        # Retorna mensagem de sucesso.
    except Exception as e:
        return JsonResponse({'mensagem': f'Erro ao enviar comando: {str(e)}'}, status=500)
        # Se ocorrer algum erro na comunicação, retorna erro 500 com o motivo.
        
        
