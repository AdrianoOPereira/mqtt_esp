import serial

try:
    arduino = serial.Serial("COM3", baudrate=115200, timeout=1)
    print("Arduino conectado!!!")
except Exception as e:
    print(f"Erro ao conectar Arduino: {e}")
    arduino = None
    
    
comandos_validos = ['TZO', 'OFF1', 'TZN', 'OFF2', 'TZS']    