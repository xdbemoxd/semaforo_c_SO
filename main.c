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
pthread_mutex_t mutex_aux_mostrador = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aux_mostrador_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aux_mostrador_3 = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_cintas;       // Turnos para las 500 cintas
pthread_mutex_t mutex_aux_cinta = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_almacenamiento; // Turnos para las áreas de almacenamiento
pthread_mutex_t mutex_aux_avion = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_aux_avion_2 = PTHREAD_MUTEX_INITIALIZER;
//listas
listaAvion aviones;
listaVuelo vuelos;
listaEquipaje equipajes;
Lista avionesDespejados;
Lista avionesMostrados;
//cola
Cola colaEquipajeTipo1;
Cola colaEquipajeTipo2;
Cola colaEquipajeTipo3;
Cola colaEquipajeTipo4;
Cola colaEquipajeTipo1Fragil;
Cola colaEquipajeTipo2Fragil;
Cola colaEquipajeTipo3Fragil;
Cola colaEquipajeTipo4Fragil;
Cola equipajePerdido;
//debuging
int cantidadVeces = 0;
Cola vueloCorruptos;
//Contadores
int catidadProcesosListos = 0;
int listo = 0;

//nodo para guardar el anterior que se busco
equipaje *anteriorAuxEquipaje;
//Tamanos 
#define numMostradores 5000
#define numCintas 500
#define numAreasAlmacenamiento 250
#define maxHilo 100

//prototipo de cargarDatos
void cargarDatos(listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes);
void cargarDatosArchivo(listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes);
int traerFragil(listaEquipaje* equipajes, int id);
int tipoEquipaje(listaEquipaje* equipajes, int id);

// Al principio del archivo, después de los includes
volatile int ejecutando = 1;

//Funciones que exije el enunciado
void* procesoMostrador( void* threadid );
void procesoCinta( int threadid, int numVuelo, int numCola );
void procesoAreaAlmacenamiento( int threadid, int numAvion, int numCola ); 
void procesoAvion( int threadid, int numAvion, int numCola );
void procesoCintaRecogida();

//Otras funciones
int equipajeEspecial(int idEqui);
void pasillo( int threadid, int numVuelo, int numCola );
int avionFull(int idAvion);


int main()
{
    //&inicializar
    equipaje *nodoAux;
    pthread_t hiloEquipaje;
    int idEquipaje;
    int i = 0;
    int procesado = 0;
    int hilos_lote = 0;

    //nodo auxiliar en NULL
    anteriorAuxEquipaje = NULL;
    //listas
    crear_LA( &aviones );
    crear_LV( &vuelos );
    crear_LE( &equipajes );
    crear_L( &avionesDespejados );
    crear_L( &avionesMostrados );

    //colas
    crearCola( &colaEquipajeTipo1 );
    crearCola( &colaEquipajeTipo2 );
    crearCola( &colaEquipajeTipo3 );
    crearCola( &colaEquipajeTipo4 );
    crearCola( &colaEquipajeTipo1Fragil );
    crearCola( &colaEquipajeTipo2Fragil );
    crearCola( &colaEquipajeTipo3Fragil );
    crearCola( &colaEquipajeTipo4Fragil );
    crearCola( &equipajePerdido );

    //semaforos
    sem_init(&sem_mostradores, 0, numMostradores);
    sem_init(&sem_cintas, 0, numCintas);
    sem_init(&sem_almacenamiento, 0, numAreasAlmacenamiento);

    // Cargar datos primero
    cargarDatos(&aviones, &vuelos, &equipajes);
    //cargarDatosArchivo(&aviones, &vuelos, &equipajes);
    //mostrar
    //mostrar_LA(aviones);
    //mostrar_LV(vuelos);
    //mostrar_LE(equipajes);

    // Reservar memoria para los hilos
    pthread_t *vectorHilo = (pthread_t*)malloc(equipajes.longitud * sizeof(pthread_t));

    if (vectorHilo == NULL) {
        printf("Error: No se pudo asignar memoria para los hilos\n");
        return 1;
    }

    // Procesar equipajes en lotes
    nodoAux = equipajes.prim;
    while ( procesado < equipajes.longitud ) 
    {
        //hilos_lote = 0;
        
        // Crear lote de hilos
        while ( hilos_lote < equipajes.longitud && nodoAux != NULL ) 
        {
            int *id = malloc( sizeof( int ) );
            if ( id != NULL) {
                *id = nodoAux->id;
                if ( pthread_create( &vectorHilo[ hilos_lote ], NULL, procesoMostrador, id ) == 0 ) 
                {
                    hilos_lote++;
                    procesado++;
                } else {
                    free( id );
                    printf( "Error al crear hilo para equipaje %d\n" , nodoAux->id );
                }
            }
            nodoAux = nodoAux->prox;
        }
        
        // Esperar a que termine el lote actual
        for ( int j = 0; j < equipajes.longitud; j++ ) 
        {
            pthread_join( vectorHilo[ j ], NULL );
        }
    }

    // Liberar memoria
    free( vectorHilo );

    //mostrar_CE(&vueloCorruptos);

    //printf("\ntamano cola1: %d\n",colaEquipajeTipo1.tamano);
    //printf("\ntamano cola2: %d\n",colaEquipajeTipo2.tamano);
    //printf("\ntamano cola3: %d\n",colaEquipajeTipo3.tamano);
    //printf("\ntamano cola4: %d\n",colaEquipajeTipo4.tamano);

    printf("\n%d veces\n", catidadProcesosListos);

    vaciar_LA( &aviones );
    vaciar_LV( &vuelos );
    vaciar_LE( &equipajes );
    printf( "Programa finalizado correctamente\n" );
    return 0;
}

void cargarDatos( listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes )
{
    char *linea = NULL;
    size_t len = 0;
    ssize_t read;
    int seccion = -1; // -1: ninguna, 0: aviones, 1: vuelos, 2: equipajes
    int contadorEquipaje=0;
    
    printf( "Iniciando carga de datos...\n" );

    while ( ( read = getline( &linea, &len, stdin ) ) != -1 ) 
    {
        // Eliminar el salto de línea final si existe
        if ( read > 0 && linea[ read - 1 ] == '\n' ) 
        {
            linea[ read - 1 ] = '\0';
        }
        
        // Ignorar líneas vacías
        if ( strlen( linea ) <= 1 ) continue;
        
        // Detectar sección por comentarios
        if ( linea[ 0 ] == '#' ) {
            if ( strstr( linea, "aviones" ) != NULL ) 
            {
                seccion = 0;
                printf( "\nProcesando aviones...\n" );
                continue;
            } 
            else if ( strstr( linea, "vuelos" ) != NULL ) 
            {
                seccion = 1;
                printf( "\nProcesando vuelos...\n" );
                continue;
            } 
            else if ( strstr( linea, "equipajes" ) != NULL ) 
            {
                seccion = 2;
                printf( "\nProcesando equipajes...\n" );
                continue;
            }
            continue;
        }

        switch( seccion ) 
        {
            case 0: 
            { // Aviones
                avion *nuevoAvion = ( avion* )malloc( sizeof( avion ) );
                if ( sscanf(linea, "%d %d %d", &nuevoAvion->id, &nuevoAvion->tipo, &nuevoAvion->capacidadCarga) == 3 ) 
                {
                    insertar_final_LA( aviones, nuevoAvion );
                    contadorEquipaje++;
                    printf( "#" );
                } 
                else 
                {
                    free( nuevoAvion );
                    printf( "Error al leer avión: %s\n" , linea );
                }
                break;
            }
            case 1: 
            { // Vuelos
                vuelo *nuevoVuelo = ( vuelo* )malloc( sizeof( vuelo ) );
                if (sscanf( linea, "%d %d %d %d", &nuevoVuelo->id, &nuevoVuelo->paisCiudad[0], &nuevoVuelo->paisCiudad[1], &nuevoVuelo->idAvion) == 4 ) 
                {
                    insertar_final_LV( vuelos, nuevoVuelo );
                    printf( "#" );
                } 
                else 
                {
                    free( nuevoVuelo );
                    printf( "Error al leer vuelo: %s\n", linea );
                }
                break;
            }
            case 2: 
            { // Equipajes
                equipaje *nuevoEquipaje = ( equipaje* )malloc( sizeof( equipaje ) );
                if ( sscanf( linea, "%d %d %d %d", &nuevoEquipaje->id, &nuevoEquipaje->tipo, &nuevoEquipaje->vuelo, &nuevoEquipaje->fragilidad ) == 4 ) 
                {
                    
                    vuelo *vueloAsignado;

                    vueloAsignado = vuelos->prim;
                    
                    while ( vueloAsignado != NULL && vueloAsignado->id != nuevoEquipaje->vuelo ) 
                    {
                        vueloAsignado = vueloAsignado->prox;
                    }

                    if (vueloAsignado != NULL) 
                    {
                        nuevoEquipaje->paisCiudad[ 0 ] = vueloAsignado->paisCiudad[ 0 ];
                        nuevoEquipaje->paisCiudad[ 1 ] = vueloAsignado->paisCiudad[ 1 ];
                    } 
                    else 
                    {
                        nuevoEquipaje->paisCiudad[ 0 ] = 0;
                        nuevoEquipaje->paisCiudad[ 1 ] = 0;
                    }
                    
                    nuevoEquipaje->tiempo_inicio = time( NULL );
                    insertar_final_LE( equipajes, nuevoEquipaje );

                    if(contadorEquipaje>0)
                    {
                        printf( "#" );
                        contadorEquipaje--;
                    }
                    
                } 
                else 
                {
                    free( nuevoEquipaje );
                    printf( "Error al leer equipaje: %s\n" , linea );
                }
                break;
            }
        }
    }
    printf( "\nCarga de datos completada.\n" );
    free( linea );  // Liberar la memoria asignada por getline
}

void cargarDatosArchivo( listaAvion *aviones, listaVuelo *vuelos, listaEquipaje *equipajes )
{
    FILE *archivoAviones;
    char lineaArchivo[ 256 ];
    int seccion = 1; // 1: vuelos, 2: equipajes
    
    printf( "Iniciando carga de datos...\n" );

    // Procesar aviones desde archivo
    printf( "\nProcesando aviones desde archivo...\n" );
    archivoAviones = fopen( "Aviones.txt", "r" );
    if ( archivoAviones == NULL ) {
        printf( "Error al abrir el archivo de aviones\n" );
        return;
    }

    while ( fgets( lineaArchivo, sizeof( lineaArchivo ), archivoAviones ) ) 
    {
        lineaArchivo[ strcspn( lineaArchivo, "\r\n" ) ] = '\0'; // Eliminar saltos
        if ( strlen( lineaArchivo ) == 0 || lineaArchivo[ 0 ] == '#' ) continue;
        
        avion *nuevoAvion = ( avion* )malloc( sizeof( avion ) );
        if (sscanf( lineaArchivo, "%d %d %d", &nuevoAvion->id, &nuevoAvion->tipo, &nuevoAvion->capacidadCarga ) == 3 ) 
        {
            insertar_final_LA( aviones, nuevoAvion );
            printf( "#" );
        } else {
            free( nuevoAvion );
            printf( "Error al leer avión: %s\n", lineaArchivo );
        }
    }
    fclose( archivoAviones );

    // Procesar vuelos y equipajes desde stdin
    printf( "\nProcesando vuelos y equipajes desde entrada estándar...\n" );
    while ( fgets( lineaArchivo, sizeof( lineaArchivo ), stdin ) ) 
    {
        lineaArchivo[ strcspn( lineaArchivo, "\r\n" ) ] = '\0';
        
        if ( strlen( lineaArchivo ) == 0 ) break; // Salir si EOF (Ctrl+Z en Windows)
        
        if ( lineaArchivo[ 0 ] == '#' ) 
        {
            if ( strstr( lineaArchivo, "vuelos" ) != NULL ) 
            {
                seccion = 1;
                printf( "\nProcesando vuelos...\n" );
            } else if ( strstr( lineaArchivo, "equipajes" ) != NULL ) 
            {
                seccion = 2;
                printf( "\nProcesando equipajes...\n" );
            }
            continue;
        }

        switch( seccion ) 
        {
            case 1: 
            { // Vuelos
                vuelo *nuevoVuelo = ( vuelo* )malloc( sizeof( vuelo ) );
                if ( sscanf(lineaArchivo, "%d %d %d %d", &nuevoVuelo->id, &nuevoVuelo->paisCiudad[0], &nuevoVuelo->paisCiudad[1], &nuevoVuelo->idAvion ) == 4 ) 
                {
                    insertar_final_LV( vuelos, nuevoVuelo );
                    printf( "#" );
                } else {
                    free( nuevoVuelo );
                    printf( "Error al leer vuelo: %s\n", lineaArchivo );
                }
                break;
            }
            case 2: 
            { // Equipajes
                equipaje *nuevoEquipaje = ( equipaje* )malloc( sizeof( equipaje ) );
                if ( sscanf( lineaArchivo, "%d %d %d %d", &nuevoEquipaje->id, &nuevoEquipaje->tipo, &nuevoEquipaje->vuelo, &nuevoEquipaje->fragilidad ) == 4 ) 
                {
                    vuelo *vueloAsignado;

                    vueloAsignado = vuelos->prim;
                    
                    while ( vueloAsignado != NULL && vueloAsignado->id != nuevoEquipaje->vuelo ) 
                    {
                        vueloAsignado = vueloAsignado->prox;
                    }

                    if ( vueloAsignado != NULL ) 
                    {
                        nuevoEquipaje->paisCiudad[ 0 ] = vueloAsignado->paisCiudad[ 0 ];
                        nuevoEquipaje->paisCiudad[ 1 ] = vueloAsignado->paisCiudad[ 1 ];
                    } else {
                        nuevoEquipaje->paisCiudad[ 0 ] = 0;
                        nuevoEquipaje->paisCiudad[ 1 ] = 0;
                    }
                    nuevoEquipaje->tiempo_inicio = time( NULL );
                    insertar_final_LE( equipajes, nuevoEquipaje );
                    printf( "#" );
                } else {
                    free( nuevoEquipaje );
                    printf( "Error al leer equipaje: %s\n", lineaArchivo );
                }
                break;
            }
        }
    }
    printf( "\nCarga de datos completada.\n" );
}

void* procesoMostrador( void* threadid )
{
    int tid = *( int* ) threadid;
    int fragil, numCola, numVuelo, equipajeEspecialVar = 1;
    equipaje *auxEquipaje = NULL;

    sem_wait( &sem_mostradores );
    
    pthread_mutex_lock( &mutex_aux_mostrador );

    pthread_mutex_lock( &mutex_aux_mostrador_2 );
    
    auxEquipaje = equipajes.prim;

    while ( auxEquipaje != NULL && auxEquipaje->id != tid ) 
    {
        auxEquipaje = auxEquipaje->prox;
    }

    if ( auxEquipaje->tipo == 1 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 10 : 11;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo1 : &colaEquipajeTipo1Fragil, tid );
    } else if ( auxEquipaje->tipo == 2 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 20 : 21;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo2 : &colaEquipajeTipo2Fragil, tid );
        equipajeEspecialVar = equipajeEspecial(tid);
    } else if ( auxEquipaje->tipo == 3 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 30 : 31;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo3 : &colaEquipajeTipo3Fragil, tid );
    } else if ( auxEquipaje->tipo == 4 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 40 : 41;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo4 : &colaEquipajeTipo4Fragil, tid );
    }

    numVuelo = auxEquipaje->vuelo;

    usleep(1000);
    pthread_mutex_unlock( &mutex_aux_mostrador_2 );
    usleep(1000);

    pthread_mutex_unlock( &mutex_aux_mostrador ); 
    
    if ( numCola == 20 || numCola ==21 )
    {
        
        if ( equipajeEspecialVar == 0 )
        {
            //printf("\nsoy especial idEqui: %d\n", tid);

            pasillo( tid, numVuelo, numCola );
        }

        //printf("\nno soy especial idEqui: %d\n", tid);
        
    }
    
    sem_wait( &sem_cintas ); 

    procesoCinta( tid, numVuelo, numCola );
    
    
}

void pasillo( int threadid, int numVuelo, int numCola )
{
    vuelo* auxVuelo;
    int idVuelo;

    pthread_mutex_lock( &mutex_aux_cinta );

    //printf("\nestoy en la seccion critica de pasillo %d\n", threadid);
    
    auxVuelo = vuelos.prim;

    while ( auxVuelo != NULL && auxVuelo->id != numVuelo) 
    {
        auxVuelo = auxVuelo->prox;
    }

    idVuelo = auxVuelo->idAvion;

    //printf("\nsali de la seccion critica de pasillo %d\n", threadid);
    
    pthread_mutex_unlock( &mutex_aux_cinta );

    procesoAvion(threadid, idVuelo, numCola);
}

int equipajeEspecial(int idEqui)
{
    int aux = 1;

    if ( idEqui % 43 == 0 ) //si es un equipaje de conexion
    {
        aux = 0;
    }

    if ( idEqui % 49 == 0 ) //si lo lleva una persona con necesidades especiales
    {
        aux = 0;
    }
    
    return aux;

}

void procesoCinta( int threadid, int numVuelo, int numCola)
{
    vuelo* auxVuelo;
    int idVuelo;

    pthread_mutex_lock( &mutex_aux_cinta );

    //printf("\nestoy en la seccion critica de cinta %d\n", threadid);
    
    auxVuelo = vuelos.prim;

    while ( auxVuelo != NULL && auxVuelo->id != numVuelo) 
    {
        auxVuelo = auxVuelo->prox;
    }

    idVuelo = auxVuelo->idAvion;

    //printf("\nsaliendo de la seccion critica de cinta %d\n", threadid);
    
    pthread_mutex_unlock( &mutex_aux_cinta );
    usleep(1000);

    sem_wait( &sem_almacenamiento );
    usleep(1000);
    procesoAreaAlmacenamiento( threadid, idVuelo, numCola );
    
}

void procesoAreaAlmacenamiento( int threadid, int numAvion, int numCola )
{
    usleep(1000);
    procesoAvion(threadid, numAvion, numCola);

}
void procesoAvion( int threadid, int numAvion, int numCola )
{
    avion* auxAvion;

    //printf("\nliberando espacio en los semaforos %d\n", threadid);

    if ( numCola == 20 || numCola ==21 )
    {
        if ( equipajeEspecial( threadid ) != 0 )
        {
            sem_post( &sem_almacenamiento );
            sem_post( &sem_cintas );
        }
    }
    else
    {
        sem_post( &sem_almacenamiento );
        sem_post( &sem_cintas );
    }
    
    sem_post( &sem_mostradores );
    usleep(1000);

    pthread_mutex_lock( &mutex_aux_avion );

    //printf("\nseccion critica de aviones %d\n", threadid);

    auxAvion = aviones.prim;

    while ( auxAvion->id != numAvion && auxAvion != NULL )
    {
        auxAvion = auxAvion->prox;
    }

    auxAvion->capacidadCarga--;

   
    if ( auxAvion->capacidadCarga == 0 )
    {
        printf("\nAvion: %d acaba de despegar\n", auxAvion->id);
        insertar_inicio_L(&avionesDespejados, auxAvion->id);
        printf("\ncantidad de procesos listos %d\n", catidadProcesosListos);
        procesoCintaRecogida();
    }else
    {
        encolar( &equipajePerdido, threadid );
    }
    
    
    catidadProcesosListos++;

    //printf("\nsaliendo de la seccion critica de aviones %d\n", threadid);

    pthread_mutex_unlock( &mutex_aux_avion );

    //usleep(1000);
    
    pthread_mutex_lock( &mutex_aux_avion_2 );


    if ( catidadProcesosListos > equipajes.longitud  && listo == 0)
    {
        procesoCintaRecogida();
        listo = 1;
    }
    
    pthread_mutex_unlock( &mutex_aux_avion_2 );
}

void procesoCintaRecogida()
{
    printf("\nestoy en cinta de entrega\n");
}