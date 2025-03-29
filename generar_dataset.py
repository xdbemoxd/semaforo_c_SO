import random

def generar_aviones():
    with open('Aviones.txt', 'w') as file:
        # Sección de aviones
        file.write("#aviones\n")
        numero_aviones = 8
        
        for id in range(1, numero_aviones):
            tipo = random.randint(1, 3)
            if tipo == 1:
                capacidad = random.randint(200, 400)
            elif tipo == 2:
                capacidad = random.randint(500, 700)
            else:
                capacidad = random.randint(700, 800)
            file.write(f"{id} {tipo} {capacidad}\n")

        lista_aviones = []
        # Sección de vuelos
        file.write("#vuelos\n")
        for id in range(1, numero_aviones):
            pais = random.randint(1, 20)
            
            if pais == 1:
                ciudad = random.randint(1, 5)
            elif pais == 2:
                ciudad = random.randint(6, 10)
            elif pais == 3:
                ciudad = random.randint(11, 15)
            elif pais == 4:
                ciudad = random.randint(16, 20)
            elif pais == 5:
                ciudad = random.randint(21, 25)
            elif pais == 6:
                ciudad = random.randint(26, 30)
            elif pais == 7:
                ciudad = random.randint(31, 35)
            elif pais == 8:
                ciudad = random.randint(36, 40)
            elif pais == 9:
                ciudad = random.randint(41, 45)
            elif pais == 10:
                ciudad = random.randint(46, 50)
            elif pais == 11:
                ciudad = random.randint(51, 55)
            elif pais == 12:
                ciudad = random.randint(56, 60)
            elif pais == 13:
                ciudad = random.randint(61, 65)
            elif pais == 14:
                ciudad = random.randint(66, 70)
            elif pais == 15:
                ciudad = random.randint(71, 75)
            elif pais == 16:
                ciudad = random.randint(76, 80)
            elif pais == 17:
                ciudad = random.randint(81, 85)
            elif pais == 18:
                ciudad = random.randint(86, 90)
            elif pais == 19:
                ciudad = random.randint(91, 95)
            else:
                ciudad = random.randint(96, 100)

            avion = random.randint(1, numero_aviones-1)

            if avion not in lista_aviones:
                lista_aviones.append(avion)
            else:
                while avion in lista_aviones:
                    avion = random.randint(1, numero_aviones-1)
                    if avion not in lista_aviones:
                        lista_aviones.append(avion)
                        break


            file.write(f"{id} {pais} {ciudad} {avion}\n")

        # Sección de equipajes
        file.write("#equipajes\n")
        numero_equipaje = 5001
        
        for i in range(1, numero_equipaje):

            tipo = random.randint(1, 4)
            idvuelo = random.randint(1, numero_aviones-1)
            fragil = random.randint(0, 1)
            file.write(f"\n{i} {tipo} {idvuelo} {fragil}")

generar_aviones()