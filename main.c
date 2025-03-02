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
sem_t sem_aux_mostrador;
sem_t sem_cintas;       // Turnos para las 500 cintas
sem_t sem_almacenamiento; // Turnos para las áreas de almacenamiento
//listas
listaAvion aviones;
listaVuelo vuelos;
listaEquipaje equipajes;
//cola
Cola colaEquipajeTipo1;
Cola colaEquipajeTipo2;
Cola colaEquipajeTipo3;
Cola colaEquipajeTipo4;
//nodo para guardar el anterior que se busco
equipaje *anteriorAuxEquipaje;
//Tamanos 
#define numMostradores 5000
#define numCintas 500
#define numAreasAlmacenamiento 250

//prototipo de cargarDatos
void cargarDatos(listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes);

// Al principio del archivo, después de los includes
volatile int ejecutando = 1;

//Funciones que exije el enunciado
void* procesoMostrador(void* threadid);
void procesoCinta();
void procesoAreaAlmacenamiento(); 
void procesoAvion();
void procesoCintaRecogida();

//Otras funciones
void datosEquipaje(ListaEquipaje l, equipaje* traerDatos, int tid, equipaje* datoAterior );

int main()
{
    //&inicializar
    equipaje *nodoAux;
    pthread_t hiloEquipaje;
    int idEquipaje;
    int i=0;
    int aux=0;

    //nodo auxiliar en NULL
    anteriorAuxEquipaje = NULL;
    //listas
    crear_LA(&aviones);
    crear_LV(&vuelos);
    crear_LE(&equipajes);

    //colas
    crearCola(&colaEquipajeTipo1);
    crearCola(&colaEquipajeTipo2);
    crearCola(&colaEquipajeTipo3);
    crearCola(&colaEquipajeTipo4);

    //semaforos
    sem_init(&sem_mostradores, 0, numMostradores);
    sem_init(&sem_aux_mostrador, 0, 1);
    
    // Cargar datos primero
    cargarDatos(&aviones, &vuelos, &equipajes);
    //mostrar
    //mostrar_LA(aviones);
    //mostrar_LV(vuelos);
    //mostrar_LE(equipajes);

    nodoAux = equipajes.prim;

    pthread_t *vectorHilo = (pthread_t*)malloc(equipajes.longitud * sizeof(pthread_t));

    while ( nodoAux != NULL)
    {
        idEquipaje = nodoAux->id;
        pthread_create(&vectorHilo[i], NULL, procesoMostrador, &idEquipaje);
        nodoAux = nodoAux->prox;
        i++;
    }

    while (aux<i)
    {
        pthread_join(vectorHilo[aux], NULL);
        aux++;
    }

    //mostrar_CE(&colaEquipajeTipo1);

    vaciar_LA(&aviones);
    vaciar_LV(&vuelos);
    vaciar_LE(&equipajes);
    printf("Programa finalizado correctamente\n");
    return 0;
}

void cargarDatos(listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes)
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

void* procesoMostrador(void* threadid)
{
    //printf("Jorge es gay");
    int tid = *( int* )threadid;

    sem_wait( &sem_mostradores );

    //printf("hilo del proceso %ld\n",tid);

    sem_wait( &sem_aux_mostrador );

    equipaje *auxEquipaje;

    datosEquipaje( equipajes, auxEquipaje, tid, anteriorAuxEquipaje );
    
    encolar( &colaEquipajeTipo1, tid );

    //printf("\ntamano cola %d\n",colaEquipajeTipo1.tamano);

    sem_post(&sem_aux_mostrador);
    

    sem_post(&sem_mostradores);

}

void datosEquipaje(ListaEquipaje l, equipaje* traerDatos, int tid, equipaje* datoAterior)
{
    equipaje *auxEquipaje;

    if ( datoAterior == NULL )
    {
        auxEquipaje = l.prim;

        if ( auxEquipaje->id == tid )
        {
            traerDatos = auxEquipaje;
        }else
        {
            while (auxEquipaje != NULL && auxEquipaje->id != tid)
            {
                //printf( "\nid: %d, Vuelo: %d, fragilidad: %d\n", auxEquipaje->id, auxEquipaje->vuelo, auxEquipaje->fragilidad );
                printf( "\nJorge es gay\n");
                if (auxEquipaje->id == tid)
                {
                    datoAterior = auxEquipaje;
                    traerDatos = auxEquipaje;
                    break;
                }

                auxEquipaje = auxEquipaje->prox;
        
            }
        }
    }else
    {
        auxEquipaje = datoAterior;

        if ( auxEquipaje->id == tid )
        {
            traerDatos = auxEquipaje;
        }else
        {
            while (auxEquipaje != NULL && auxEquipaje->id != tid)
            {
                printf( "\nRafael es gay\n");
                if (auxEquipaje->id == tid)
                {
                    traerDatos = auxEquipaje;
                    break;
                }

                auxEquipaje = auxEquipaje->prox;
        
            }
        }
        
    }
    
    
}