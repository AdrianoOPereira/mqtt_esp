import serial
import time
import os

arduino = serial.Serial('COM3') # COMxx   format on Windows
                                  # ttyUSBx format on Linux

#arduino.baudrate = 115200  # set Baud rate to 115200
#arduino.bytesize = 8     # Number of data bits = 8
#arduino.parity   ='N'    # No parity
#arduino.stopbits = 1     # Number of Stop bits = 1
#time.sleep(3)
#print(arduino)
#print('Porta com em funcionamento...')

while True:
    try:
        arduino.baudrate = 115200  # set Baud rate to 115200
        arduino.bytesize = 8     # Number of data bits = 8
        arduino.parity   ='N'    # No parity
        arduino.stopbits = 1 
        print("Arduino conectado!!!")
        break
    except:
        pass
    
while True:
    cmd = input("Digite uma rota...")
    
    if cmd == 'TZO':
        arduino.write('TZO'.encode())
        print('Lampada acesa!')
    elif cmd == 'OFF1':
        arduino.write('OFF1'.encode())
        print('Lampada apagada!')   
         
    elif cmd == 'TZN':
        arduino.write('TZN'.encode())
        print('Lampada acesa!')
    elif cmd == 'OFF2':
        arduino.write('OFF2'.encode())
        print('Lampada apagada!')
        
    arduino.flush()            