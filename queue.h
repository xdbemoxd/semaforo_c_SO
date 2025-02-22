#ifndef Cola_H
#define Cola_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Cliente {
    int id;
    bool vip;
    bool retiro;
    int m2;
    int diaLleg;
    int tiempoRet;
    int horaLleg;
    struct Cliente* siguiente;
} Cliente;


typedef struct Cola {
    Cliente* frente;
    Cliente* trasero;
    int tamano;
} Cola;


int longitud(Cola* cola) {
    return cola->tamano;
}

void crearCola(Cola* cola) {
    cola->frente = NULL;
    cola->trasero = NULL;
    cola->tamano = 0;
}

int  esVacio(Cola* cola) {
  return cola->tamano == 0;
}

void encolar(Cola* cola, Cliente* cliente) {
    cliente->siguiente = NULL;
    if (esVacio(cola)) {
      cola->frente  = cola -> trasero= cliente;
    } else {
      cola->trasero->siguiente = cliente;
    }
    cola->trasero = cliente;
    cola->tamano++;
}

Cliente* desencolar(Cola* cola) {
    if (esVacio(cola)) {
        return NULL;
    }
    Cliente* cliente = cola->frente;
    cola->frente = cola->frente->siguiente;
    if (cola->frente == NULL) {
        cola->trasero = NULL;
    }
    cola->tamano--;
    return cliente;
}

// int size(Cola* cola) {
//     return cola->tamano;
// }

// int size(Cola *q){
//   Nodo *nuevoNodo = q -> frente;
//   int resultado = 0;
//   while(nuevoNodo != NULL){
//     resultado++;
//     nuevoNodo = nuevoNodo -> siguiente;
//   }
//   return resultado;
// }


// void crearCola(Cola *q){
//   q -> frente = q -> trasero = NULL;
// }

// int esVacio(Cola *q){
//   return (q -> frente == NULL)? 1:0;
// }

// void encolar(Cola *q, Cliente *infoCliente) {
//     Nodo *nuevoNodo = (Nodo*)malloc(sizeof(Nodo));

//     if (nuevoNodo == NULL) { // Para verificar que fue creado bien
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }

//     nuevoNodo -> info = infoCliente;
//     nuevoNodo -> siguiente = NULL;
    
//     if (q -> trasero == NULL) {
//         // La cola está vacía
//         q -> frente = nuevoNodo;
//         q -> trasero = nuevoNodo;
//     } else {
//         // Agregar al final de la cola
//         q -> trasero -> siguiente = nuevoNodo;
//         q -> trasero = nuevoNodo;
//     }
// }

// Cliente *desencolar(Cola *q) {
//     // if (q -> frente == NULL) {
//     //     // Cola vacía
//     //     return (Cliente)0;
//     // }
    
//     Nodo *temp = q -> frente;
//     Cliente *infoCliente = temp -> info;
    
//     q -> frente = q -> frente -> siguiente;
//     if (q -> frente == NULL) {
//         // La cola está vacía después de la eliminación
//         q -> trasero = NULL;
//     }
    
//     free(temp);
//     return infoCliente;
// }

#endif

// #ifndef Cola_H
// #define Cola_H

// #include <stdlib.h>
// #include <stdbool.h>

// typedef struct Cliente {
//     int id;
//     bool vip;
//     bool retiro;
//     int m2;
//     int exito;
//     float costo;
//     int diaIng;
//     int tiempoRet;
//     int horaLleg;
//     struct Cliente* siguiente;
// } Cliente;


// typedef struct Cola {
//     Cliente* frente;
//     Cliente* trasero;
//     int tamano;
// } Cola;


// int longitud(Cola* cola) {
//     return cola->tamano;
// }

// void crearCola(Cola* cola) {
//     cola->frente = NULL;
//     cola->trasero = NULL;
//     cola->tamano = 0;
// }

// int  esVacio(Cola* cola) {
//   return cola->tamano == 0;
// }

// void encolar(Cola* cola, Cliente* cliente) {
//     cliente->siguiente = NULL;
//     if (esVacio(cola)) {
//       cola->frente  = cola -> trasero= cliente;
//     } else {
//       cola->trasero->siguiente = cliente;
//     }
//     cola->trasero = cliente;
//     cola->tamano++;
// }

// Cliente* desencolar(Cola* cola) {
//     if (esVacio(cola)) {
//         return NULL;
//     }
//     Cliente* cliente = cola->frente;
//     cola->frente = cola->frente->siguiente;
//     if (cola->frente == NULL) {
//         cola->trasero = NULL;
//     }
//     cola->tamano--;
//     return cliente;
// }

// // int size(Cola* cola) {
// //     return cola->tamano;
// // }

// // int size(Cola *q){
// //   Nodo *nuevoNodo = q -> frente;
// //   int resultado = 0;
// //   while(nuevoNodo != NULL){
// //     resultado++;
// //     nuevoNodo = nuevoNodo -> siguiente;
// //   }
// //   return resultado;
// // }


// // void crearCola(Cola *q){
// //   q -> frente = q -> trasero = NULL;
// // }

// // int esVacio(Cola *q){
// //   return (q -> frente == NULL)? 1:0;
// // }

// // void encolar(Cola *q, Cliente *infoCliente) {
// //     Nodo *nuevoNodo = (Nodo*)malloc(sizeof(Nodo));

// //     if (nuevoNodo == NULL) { // Para verificar que fue creado bien
// //         perror("malloc");
// //         exit(EXIT_FAILURE);
// //     }

// //     nuevoNodo -> info = infoCliente;
// //     nuevoNodo -> siguiente = NULL;
    
// //     if (q -> trasero == NULL) {
// //         // La cola está vacía
// //         q -> frente = nuevoNodo;
// //         q -> trasero = nuevoNodo;
// //     } else {
// //         // Agregar al final de la cola
// //         q -> trasero -> siguiente = nuevoNodo;
// //         q -> trasero = nuevoNodo;
// //     }
// // }

// // Cliente *desencolar(Cola *q) {
// //     // if (q -> frente == NULL) {
// //     //     // Cola vacía
// //     //     return (Cliente)0;
// //     // }
    
// //     Nodo *temp = q -> frente;
// //     Cliente *infoCliente = temp -> info;
    
// //     q -> frente = q -> frente -> siguiente;
// //     if (q -> frente == NULL) {
// //         // La cola está vacía después de la eliminación
// //         q -> trasero = NULL;
// //     }
    
// //     free(temp);
// //     return infoCliente;
// // }

// #endif
