#include <stdio.h>
#include <time.h>
#ifndef equipaje_h
#define equipaje_h

typedef struct equi{
    int id;
    int tipo;      // 1) facturado, 2) mano, 3) especial, 4) sobredimensionado
    int paisCiudad[2]; //(pais,ciudad) del destino a donde van a llegar 
    int vuelo; // vuelo al que corresponde el equipaje
    int fragilidad; //determina si es una algo qie debe tener un cuidado especial
    time_t tiempo_inicio;
    struct equi *prox; //apuntador al siguiente equipaje
} equipaje, Equipaje;

#endif 