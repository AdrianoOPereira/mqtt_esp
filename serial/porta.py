import serial
import time
import os

def conectar_arduino():
    while True:
        try:
            arduino = serial.Serial("COM3")
            arduino.baudrate = 115200  # set Baud rate to 115200
            arduino.bytesize = 8  # Number of data bits = 8
            arduino.parity = "N"  # No parity
            arduino.stopbits = 1
            print("Arduino conectado!!!")
            print(arduino)
            break
        except:
            pass

arduino = conectar_arduino()

########################################################################################

def rotas_cadastradas():
    print(rotas_cadastradas)
    while True:
        cmd = input("Digite o nome da rota...")

        if cmd == "TZO":
            arduino.write("TZO".encode())
            print(f"Lâmpada acesa!")
        elif cmd == "OFF1":
            arduino.write("OFF1".encode())
            print(f"Lâmpada apagada!")

        elif cmd == "TZN":
            arduino.write("TZN".encode())
            print(f"Lâmpada acesa!")
        elif cmd == "OFF2":
            arduino.write("OFF2".encode())
            print(f"Lâmpada apagada!")  
        else: 
            print("Rota não está cadastrada, por favor acionar o TI...")     
            
        arduino.flush()

if __name__ == "__main__":
    rotas_cadastradas()