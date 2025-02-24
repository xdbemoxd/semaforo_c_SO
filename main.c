#include <stdio.h>
#include "lista.h"
#include "nodo.h"
#include "queue.h"
#include <time.h>

#define numMostradores 5000

#define numCintas 500

#define numAreasAlmacenamiento 250

typedef struct avi{
    int id;  //identificador de avion
    int tipo;      // "modelo de avion"
    int capacidadCarga;  //equipaje maximo que puede llevar el avion
    struct avi *prox; //apuntador a avion 
} avion;

typedef struct equi{
    int id;
    int tipo;      // 1) facturado, 2) mano, 3) especial, 4) sobredimensionado
    int paisCiudad[2]; //(pais,ciudad) del destino a donde van a llegar 
    int vuelo; // vuelo al que corresponde el equipaje
    int fragilidad; //determina si es una algo qie debe tener un cuidado especial
    struct equi *prox; //apuntador a avion 
    time_t tiempo_inicio;
} equipaje;

typedef struct vue{
    int id;  //identificador de avion
    int paisCiudad[2];  // "modelo de avion"
    int idAvion;  //equipaje maximo que puede llevar el avion
    struct vue *prox; //apuntador a avion 
} vuelo;

void mostrador();
void cinta();
void áreaAlmacenamiento();
void avion();
void cintaRecogida();



/*
1) se debe crear una cola para priorizar los equipajes, para darle prioridad a los que necesitan

2) se debe tener una lista de vuelos y sus respectivos datos

3) organizar la entrada standart y como se ingresaran los datos
*/


int main()
{

    printf("are you gay benjamon?");

    return 0;
}