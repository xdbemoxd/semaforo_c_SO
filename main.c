#include <stdio.h>
#include "lista.h"
#include "nodo.h"
#include "queue.h"
#include <time.h>

#define numMostradores 5000

#define numCintas 500

#define numAreasAlmacenamiento 250
/*
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

void tomarDatosAviones();
void tomarDatosVuelos();
void tomarDatosEquipaje();


1) se debe crear una cola para priorizar los equipajes, para darle prioridad a los que necesitan

2) se debe tener una lista de vuelos y sus respectivos datos

3) organizar la entrada standart y como se ingresaran los datos

[23/2/2025 21:32] Benjamin Marroquin: aviones = equipaje/3
[23/2/2025 21:32] Benjamin Marroquin: para evitar que algún avión despeje sin estar totalmente lleno
[23/2/2025 21:32] Benjamin Marroquin: o poner una restricción que si después de recibir equipaje tarda x cantidad de tiempo, el avión desoeje
*/

int main()
{

    printf("are you gay benjamon?");

    return 0;
}

void tomarDatosAviones()
{
    int idAvione,tipoAvion,cantidadPasajeros,cantidadAviones,i;

    printf("\nAviones\n");
    scanf("%d", &cantidadAviones);

    for(i=0; i<cantidadAviones-1; i++)
    {
        scanf("%d %d %d", &idAvione, &tipoAvion, &cantidadPasajeros);
        printf("%d %d %d\n", idAvione, tipoAvion, cantidadPasajeros);
    }
}

void tomarDatosVuelos()
{
    int idVuelo,pais,ciudad,idAv,j,cantidadVuelos;
    printf("\nVuelos\n");
    scanf("%d", &cantidadVuelos);
    
    printf("\ncantidad de Vuelos:%d\n",cantidadVuelos);

    for(j=0; j<cantidadVuelos-1; j++)
    {
        printf("\nVuelo:%d\n",j);
        scanf("%d %d %d %d", &idVuelo,&pais,&ciudad,&idAv);
        printf("%d %d %d %d\n", idVuelo, pais, ciudad, idAv);
    }

}

void tomarDatosEquipaje()
{
    int idEquipaje,tipoEquipaje,Fragil,idVuelo,k,cantidadEquipaje;

    printf("\nEquipaje\n");
    scanf("%d", &cantidadEquipaje);

    for(k=0; k<cantidadEquipaje-1; k++)
    {
        scanf("%d %d %d %d", &idEquipaje,&tipoEquipaje,&idVuelo,&Fragil);
        printf("%d %d %d %d\n", idEquipaje,tipoEquipaje,idVuelo,Fragil);
    }

}