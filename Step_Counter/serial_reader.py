import serial
import csv
import re

# Configurar a porta serial (ajuste a porta e a taxa de transmissão conforme necessário)
ser = serial.Serial('COM7', 115200)

# Expressões regulares para capturar os valores de x, y, z
regex_x = re.compile(r'x:\s*(-?\d+\.?\d*)')
regex_y = re.compile(r'y:\s*(-?\d+\.?\d*)')
regex_z = re.compile(r'z:\s*(-?\d+\.?\d*)')

# Abrir um arquivo CSV para escrita
with open('dados.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Contagem de Pulsos por Eixo', 'x', 'y', 'z'])  # Cabeçalho da coluna

    try:
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').rstrip()
                print(line)

                # Inicializar os valores como None
                x_value = None
                y_value = None
                z_value = None

                # Encontrar os valores de x, y e z na linha
                match_x = regex_x.search(line)
                match_y = regex_y.search(line)
                match_z = regex_z.search(line)

                if match_x:
                    x_value = match_x.group(1)
                if match_y:
                    y_value = match_y.group(1)
                if match_z:
                    z_value = match_z.group(1)

                # Escrever os valores na linha do CSV
                writer.writerow([line, x_value, y_value, z_value])

    except KeyboardInterrupt:
        print("Interrompido pelo usuário")
    finally:
        ser.close()