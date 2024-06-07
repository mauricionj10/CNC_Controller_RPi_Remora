import serial
import csv

# Configurar a porta serial (ajuste a porta e a taxa de transmissão conforme necessário)
ser = serial.Serial('COM3', 115200)

# Abrir um arquivo CSV para escrita
with open('dados.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Dados'])  # Cabeçalho da coluna

    try:
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                print(line)
                writer.writerow([line])
    except KeyboardInterrupt:
        print("Interrompido pelo usuário")
    finally:
        ser.close()
