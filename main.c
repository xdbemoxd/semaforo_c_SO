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
#include "ArbolEnario.h"
#include <string.h>
#include <stdarg.h> 

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
pthread_mutex_t mutex_aux_avion_3 = PTHREAD_MUTEX_INITIALIZER;
//listas
listaAvion aviones;
listaVuelo vuelos;
listaEquipaje equipajes;
Lista avionesDespejados;
Lista avionesMostrados;
ArbolEnarioAux ordenEquipaje;
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
#define maxHilo 1000
#define limpiarSemyMutex 5000
// Variables para estadísticas
int num_mostradores_libres = numMostradores;
int num_cintas_libres = numCintas;
int num_areas_libres = numAreasAlmacenamiento;
int equipaje_perdido = 0;
int equipajes_fragiles_procesados = 0;
int equipajes_especiales_procesados = 0;
int equipajes_por_tipo[4] = {0, 0, 0, 0};
time_t tiempo_inicio_global;
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
void procesoCintaRecogida( int numAvion );

//Otras funciones
void log_mensaje(const char* tipo, const char* formato, ...);
int equipajeEspecial(int idEqui);
void pasillo( int threadid, int numVuelo, int numCola );
int avionFull(int idAvion);
void mostrar_estadisticas(int procesado, int total);
void pausar_programa();
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
    arbolEnarioConstructorAviones( &ordenEquipaje );

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
// Registrar tiempo de inicio global
    tiempo_inicio_global = time(NULL);
    log_mensaje("INFO", "Iniciando simulación de gestión de equipajes");
    // Cargar datos primero
    cargarDatos(&aviones, &vuelos, &equipajes);
    //cargarDatosArchivo(&aviones, &vuelos, &equipajes);
    //mostrar
    //mostrar_LA(aviones);
    //mostrar_LV(vuelos);
    //mostrar_LE(equipajes);

    // Reservar memoria para los hilos
    pthread_t *vectorHilo = (pthread_t*)malloc(maxHilo * sizeof(pthread_t));

    if (vectorHilo == NULL) 
    {
        printf("Error: No se pudo asignar memoria para los hilos\n");
        return 1;
    }

    // Procesar equipajes en lotes
    nodoAux = equipajes.prim;
    while (procesado < equipajes.longitud) 
    {
        hilos_lote = 0;
        
        // Crear lote de hilos (máximo maxHilo a la vez)
        while (hilos_lote < maxHilo && procesado < equipajes.longitud && nodoAux != NULL) 
        {
            int *id = malloc(sizeof(int));
            if (id != NULL) {
                *id = nodoAux->id;
                if (pthread_create(&vectorHilo[hilos_lote], NULL, procesoMostrador, id) == 0) 
                {
                    hilos_lote++;
                    procesado++;
                } 
                else 
                {
                    free(id);
                    log_mensaje("ERROR", "Error al crear hilo para equipaje %d", nodoAux->id);
                }
            }
            nodoAux = nodoAux->prox;
        }
        // Mostrar progreso
        // Mostrar progreso mejorado
        printf("\rProcesando equipajes: %d/%d (%.1f%%) | Mostradores libres: %d | Cintas libres: %d | Áreas libres: %d", 
               procesado, equipajes.longitud, (float)procesado/equipajes.longitud * 100,
               num_mostradores_libres, num_cintas_libres, num_areas_libres);
        fflush(stdout);
        // Esperar a que termine el lote actual
        for (int j = 0; j < hilos_lote; j++) 
        {
            pthread_join(vectorHilo[j], NULL);
        }
        
        // Pausa entre lotes para liberar recursos
        usleep(50000);  // 50ms de pausa

        // Cada 5000 equipajes procesados, liberar completamente los semáforos
        if (procesado % limpiarSemyMutex == 0||procesado >= equipajes.longitud)
        {
            log_mensaje("SISTEMA", "Limpiando semáforos y mutexes...");
            
            // Destruir y reiniciar semáforos
            sem_destroy(&sem_mostradores);
            sem_destroy(&sem_cintas);
            sem_destroy(&sem_almacenamiento);
            
            sem_init(&sem_mostradores, 0, numMostradores);
            sem_init(&sem_cintas, 0, numCintas);
            sem_init(&sem_almacenamiento, 0, numAreasAlmacenamiento);
            
            // Pausa más larga para asegurar la liberación completa
            usleep(200000);  // 200ms
        }
    }
    printf("\n");
    mostrar_estadisticas(procesado, equipajes.longitud);
    // Liberar memoria
    free( vectorHilo );

    //printf("\n%d veces\n", catidadProcesosListos);
    //mostrar_L(avionesDespejados);
    
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
                printf( "\nCargando datos de aviones...\n" );
                continue;
            } 
            else if ( strstr( linea, "vuelos" ) != NULL ) 
            {
                seccion = 1;
                printf( "\nCargando datos de vuelos...\n" );
                continue;
            } 
            else if ( strstr( linea, "equipajes" ) != NULL ) 
            {
                seccion = 2;
                printf( "\nCargando datos de equipajes...\n" );
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
                    printf("\rCargando aviones: %d",aviones->longitud);
                    fflush(stdout);
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
                    printf("\rCargando vuelos: %d",vuelos->longitud);
                    fflush(stdout);
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
                    printf("\rCargando equipajes: %d",equipajes->longitud);
                    fflush(stdout);
                    
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
    if ( archivoAviones == NULL ) 
    {
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
                } 
                else 
                {
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
    free(threadid);  // Liberar memoria ID
    log_mensaje("EQUIPAJE", "Equipaje %d entrando al mostrador", tid);
    int fragil, numCola, numVuelo, equipajeEspecialVar = 1;
    equipaje *auxEquipaje = NULL;

    sem_wait( &sem_mostradores );
    num_mostradores_libres--;
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
    } 
    else if ( auxEquipaje->tipo == 2 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 20 : 21;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo2 : &colaEquipajeTipo2Fragil, tid );
        equipajeEspecialVar = equipajeEspecial(tid);
    } 
    else if ( auxEquipaje->tipo == 3 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 30 : 31;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo3 : &colaEquipajeTipo3Fragil, tid );
    } 
    else if ( auxEquipaje->tipo == 4 )
    {
        numCola = auxEquipaje->fragilidad == 0 ? 40 : 41;
        encolar( auxEquipaje->fragilidad == 0 ? &colaEquipajeTipo4 : &colaEquipajeTipo4Fragil, tid );
    }
    if (auxEquipaje->tipo >= 1 && auxEquipaje->tipo <= 4) {
        equipajes_por_tipo[auxEquipaje->tipo-1]++;
    }
    
    // Registrar si es frágil
    if (auxEquipaje->fragilidad == 1) {
        equipajes_fragiles_procesados++;
    }

    numVuelo = auxEquipaje->vuelo;

    usleep(1000);
    pthread_mutex_unlock( &mutex_aux_mostrador_2 );
    usleep(1000);

    pthread_mutex_unlock( &mutex_aux_mostrador ); 
    
    if ( numCola == 20 || numCola == 21 )
    {
        if ( equipajeEspecialVar == 0 )
        {
            pasillo( tid, numVuelo, numCola );
            // IMPORTANTE: Terminar aquí para evitar doble procesamiento
        }
        else
        {
            sem_wait( &sem_cintas );
            procesoCinta( tid, numVuelo, numCola );
        }
    }
    else
    {
        sem_wait( &sem_cintas ); 
        procesoCinta( tid, numVuelo, numCola );
    }
}

void pasillo( int threadid, int numVuelo, int numCola )
{
    vuelo* auxVuelo;
    int idVuelo;

    pthread_mutex_lock( &mutex_aux_cinta );
    
    auxVuelo = vuelos.prim;

    while ( auxVuelo != NULL && auxVuelo->id != numVuelo) 
    {
        auxVuelo = auxVuelo->prox;
    }

    idVuelo = auxVuelo->idAvion;
    
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
    log_mensaje("EQUIPAJE", "Equipaje %d entrando a cinta (Vuelo %d, Cola %d)", threadid, numVuelo, numCola);
    
    pthread_mutex_lock( &mutex_aux_cinta );
    num_cintas_libres--;
    auxVuelo = vuelos.prim;

    while ( auxVuelo != NULL && auxVuelo->id != numVuelo) 
    {
        auxVuelo = auxVuelo->prox;
    }

    idVuelo = auxVuelo->idAvion;
    
    pthread_mutex_unlock( &mutex_aux_cinta );
    usleep(1000);

    sem_wait( &sem_almacenamiento );
    num_areas_libres--;
    log_mensaje("EQUIPAJE", "Equipaje %d pasando a área de almacenamiento (Avión %d)", threadid, idVuelo);
    usleep(1000);
    procesoAreaAlmacenamiento( threadid, idVuelo, numCola );
    
}

void procesoAreaAlmacenamiento( int threadid, int numAvion, int numCola )
{
    usleep(1000);
    log_mensaje("EQUIPAJE", "Equipaje %d en área de almacenamiento para Avión %d", threadid, numAvion);
    procesoAvion(threadid, numAvion, numCola);

}
void procesoAvion( int threadid, int numAvion, int numCola )
{
    avion* auxAvion;

    if ( numCola == 20 || numCola ==21 )
    {
        if ( equipajeEspecial( threadid ) != 0 )
        {
            sem_post( &sem_almacenamiento );
            num_areas_libres++;
            sem_post( &sem_cintas );
            num_cintas_libres++;
        }
    }
    else
    {
        sem_post( &sem_almacenamiento );
        num_areas_libres++;
        sem_post( &sem_cintas );
        num_cintas_libres++;
    }
    
    sem_post( &sem_mostradores );
    num_mostradores_libres++;
    usleep(1000);

    pthread_mutex_lock( &mutex_aux_avion );

    pthread_mutex_lock( &mutex_aux_avion_3 );

    auxAvion = aviones.prim;

    while ( auxAvion->id != numAvion && auxAvion != NULL )
    {
        auxAvion = auxAvion->prox;

    }
    
    auxAvion->capacidadCarga--;
    catidadProcesosListos++;
    
    if ( auxAvion->capacidadCarga > 0)
    {
        if ( existeAvion( &ordenEquipaje, auxAvion->id ) == 0 )
        {
            insertarHijo( &ordenEquipaje, &equipajes, auxAvion->id, threadid );
        }
        else
        {
            insertarAvion( &ordenEquipaje, auxAvion->id );
            insertarHijo( &ordenEquipaje, &equipajes, auxAvion->id, threadid );
        }
    }
    else
    {
        if ( auxAvion->capacidadCarga == 0 )
        {
            log_mensaje("AVIÓN", "AVIÓN %d COMPLETAMENTE CARGADO", numAvion);
            insertar_inicio_L(&avionesDespejados, auxAvion->id);
            insertarHijo( &ordenEquipaje, &equipajes, auxAvion->id, threadid );
            procesoCintaRecogida( numAvion );
        }
        else if ( auxAvion->capacidadCarga < 0 )
        {
            log_mensaje("EQUIPAJE", "Equipaje %d PERDIDO - Avión %d sin capacidad", threadid, numAvion);
            equipaje_perdido++;
            encolar( &equipajePerdido, threadid );
        }
        else
        {
            log_mensaje("EQUIPAJE", "Equipaje %d cargado en Avión %d (Capacidad restante: %d)", 
                   threadid, numAvion, auxAvion->capacidadCarga);
        }
    }
    

    pthread_mutex_unlock( &mutex_aux_avion_3 );

    pthread_mutex_unlock( &mutex_aux_avion );
    
    pthread_mutex_lock( &mutex_aux_avion_2 );

    if ( catidadProcesosListos == equipajes.longitud )
    {
        int auxEntero = 1;
        
        while ( aviones.longitud >= auxEntero )
        {
            if ( buscar_L( avionesDespejados, auxEntero ) == 1 )
            {
                insertar_inicio_L( &avionesDespejados, auxEntero );
                    
                procesoCintaRecogida( auxEntero );
            }
            auxEntero++;  
        }
        

    }

    pthread_mutex_unlock( &mutex_aux_avion_2 );
    
    
}

void procesoCintaRecogida( int numAvion )
{
    log_mensaje("RECOGIDA", "Iniciando recogida de equipajes del Avión %d", numAvion);
    mostrarEquipajesAvion( ordenEquipaje, numAvion );
    //pausar_programa();
}
void log_mensaje(const char* tipo, const char* formato, ...) {
    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", localtime(&now));
    
     // Usamos un mutex para evitar que los mensajes se mezclen
    static pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex_log);

    printf("[%s][%s] ", timestamp, tipo);
    
    va_list args;
    va_start(args, formato);
    vprintf(formato, args);
    va_end(args);
    printf("\n");
    // Asegurarnos que la salida se muestre inmediatamente
    fflush(stdout);
    
    pthread_mutex_unlock(&mutex_log);
    
}
void mostrar_estadisticas(int procesado, int total) {
    float porcentaje = (float)procesado/total * 100;
    time_t now = time(NULL);
    double tiempo_transcurrido = difftime(now, tiempo_inicio_global);
    
    printf("\n=== ESTADÍSTICAS (%.1f%% completado) ===\n", porcentaje);
    printf("- Equipajes procesados: %d/%d\n", procesado, total);
    printf("- Tiempo transcurrido: %.1fs\n", tiempo_transcurrido);
    printf("- Tiempo promedio por equipaje: %.2fs\n", procesado > 0 ? tiempo_transcurrido/procesado : 0);
    printf("- Equipajes por tipo: Tipo 1: %d, Tipo 2: %d, Tipo 3: %d, Tipo 4: %d\n", 
           equipajes_por_tipo[0], equipajes_por_tipo[1], equipajes_por_tipo[2], equipajes_por_tipo[3]);
    printf("- Recursos disponibles: Mostradores: %d/%d, Cintas: %d/%d, Áreas: %d/%d\n", 
           num_mostradores_libres, numMostradores, num_cintas_libres, numCintas, num_areas_libres, numAreasAlmacenamiento);
    printf("- Aviones despegados: %d\n", avionesDespejados.longitud);
    printf("- Equipajes perdidos: %d\n", equipaje_perdido);
}
void pausar_programa()
{
    int N=6;
    printf("Pausando por %d segundos...\n",N);
    fflush(stdout);
    sleep(N);  // Pausa por N segundos
}