#include <stdio.h>
#ifndef avion_h
#define avion_h

typedef struct avi{
    int id;  //identificador de avion
    int tipo;      // "modelo de avion"
    int capacidadCarga;  //equipaje maximo que puede llevar el avion
    struct avi *prox; //apuntador al siguiente avion
} avion, Avion;

#endif 