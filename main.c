#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>     // Para rand()
#include <unistd.h>     // Para usleep()
#include "lista.h"
#include "queue.h"
#include "avion.h"
#include "equipaje.h"
#include "vuelo.h"

#include <string.h>

//Semaforos
sem_t sem_mostradores;       // Turnos para los 5000 mostradores
sem_t sem_cintas[500];       // Turnos para las 500 cintas
sem_t sem_almacenamiento[250]; // Turnos para las áreas de almacenamiento

//prototipo de cargarDatos
void cargarDatos(listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes);

// Al principio del archivo, después de los includes
volatile int ejecutando = 1;

int main()
{
    //&inicializar
    //listas/colas
    // Cargar datos primero
    cargarDatos(&aviones, &vuelos, &equipajes);
    //mostrar

    listaAvion aviones;
    listaVuelo vuelos;
    listaEquipaje equipajes;
    vaciar_LA(&aviones);
    vaciar_LV(&vuelos);
    vaciar_LE(&equipajes);
    
    printf("Programa finalizado correctamente\n");
    return 0;
}
    mostrar_LA(aviones);
    mostrar_LV(vuelos);
{
    char *linea = NULL;
    size_t len = 0;
    ssize_t read;
    int seccion = -1; // -1: ninguna, 0: aviones, 1: vuelos, 2: equipajes
    int contadorEquipaje=0;
    
    printf("Iniciando carga de datos...\n");

    while ((read = getline(&linea, &len, stdin)) != -1) 
    {
        // Eliminar el salto de línea final si existe
        if (read > 0 && linea[read-1] == '\n') 
        {
            linea[read-1] = '\0';
        }
        
        // Ignorar líneas vacías
        if (strlen(linea) <= 1) continue;
        
        // Detectar sección por comentarios
        if (linea[0] == '#') {
            if (strstr(linea, "aviones") != NULL) 
            {
                seccion = 0;
                printf("\nProcesando aviones...\n");
                continue;
            } 
            else if (strstr(linea, "vuelos") != NULL) 
            {
                seccion = 1;
                printf("\nProcesando vuelos...\n");
                continue;
            } 
            else if (strstr(linea, "equipajes") != NULL) 
            {
                seccion = 2;
                printf("\nProcesando equipajes...\n");
                continue;
            }
            continue;
        }

        switch(seccion) 
        {
            case 0: 
            { // Aviones
                avion *nuevoAvion = (avion*)malloc(sizeof(avion));
                if (sscanf(linea, "%d %d %d", 
                    &nuevoAvion->id, 
                    &nuevoAvion->tipo, 
                    &nuevoAvion->capacidadCarga) == 3) 
                {
                    insertar_final_LA(aviones, nuevoAvion);
                    contadorEquipaje++;
                    printf("#");
                } 
                else 
                {
                    free(nuevoAvion);
                    printf("Error al leer avión: %s\n", linea);
                }
                break;
            }
            case 1: 
            { // Vuelos
                vuelo *nuevoVuelo = (vuelo*)malloc(sizeof(vuelo));
                if (sscanf(linea, "%d %d %d %d", 
                    &nuevoVuelo->id, 
                    &nuevoVuelo->paisCiudad[0], 
                    &nuevoVuelo->paisCiudad[1], 
                    &nuevoVuelo->idAvion) == 4) 
                {
                    insertar_final_LV(vuelos, nuevoVuelo);
                    printf("#");
                } 
                else 
                {
                    free(nuevoVuelo);
                    printf("Error al leer vuelo: %s\n", linea);
                }
                break;
            }
            case 2: 
            { // Equipajes
                equipaje *nuevoEquipaje = (equipaje*)malloc(sizeof(equipaje));
                if (sscanf(linea, "%d %d %d %d", 
                    &nuevoEquipaje->id, 
                    &nuevoEquipaje->tipo, 
                    &nuevoEquipaje->vuelo, 
                    &nuevoEquipaje->fragilidad) == 4) 
                {
                    
                    vuelo *vueloAsignado = buscar_vuelo_por_id(*vuelos, nuevoEquipaje->vuelo);
                    if (vueloAsignado != NULL) 
                    {
                        nuevoEquipaje->paisCiudad[0] = vueloAsignado->paisCiudad[0];
                        nuevoEquipaje->paisCiudad[1] = vueloAsignado->paisCiudad[1];
                    } 
                    else 
                    {
                        nuevoEquipaje->paisCiudad[0] = 0;
                        nuevoEquipaje->paisCiudad[1] = 0;
                    }
                    
                    nuevoEquipaje->tiempo_inicio = time(NULL);
                    insertar_final_LE(equipajes, nuevoEquipaje);

                    if(contadorEquipaje>0)
                    {
                        printf("#");
                        contadorEquipaje--;
                    }
                    
                } 
                else 
                {
                    free(nuevoEquipaje);
                    printf("Error al leer equipaje: %s\n", linea);
                }
                break;
            }
        }
    }

    printf("\nCarga de datos completada.\n");
    free(linea);  // Liberar la memoria asignada por getline
}