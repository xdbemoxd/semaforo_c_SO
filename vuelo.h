#include <stdio.h>
#ifndef vuelo_h
#define vuelo_h

typedef struct vue{
    int id;  //identificador de vuelo
    int paisCiudad[2];  // (pais,ciudad) del destino
    int idAvion;  //identificador del avion asignado
    struct vue *prox; //apuntador al siguiente vuelo
} vuelo, Vuelo;

#endif 