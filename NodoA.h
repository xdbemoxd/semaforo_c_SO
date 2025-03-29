#ifndef NODO_A_H
#define NODO_A_H

#include <stdio.h>
#include <stdlib.h>
#include "equipaje.h"

// Estructura para el nodo del árbol
typedef struct NodoA {
	int dato;
	struct NodoA* hijoIzquierdo;
	struct NodoA* hijoDerecho;
} NodoA;

typedef struct ApuntadorEquipaje
{
	struct ApuntadorEquipaje* siguiente;
	equipaje* senalaEqui;
}ApuntadorEquipaje;

typedef struct NodoArbolAvion
{
	int numAvion;
	struct NodoArbolAvion* hermano;
	ApuntadorEquipaje* primero;
}NodoArbolAvion;


// Declaraciones de funciones
NodoA* NodoA_Constructor(void);
NodoA* NodoA_ConstructorConDato(int NuevoDato);
void NodoA_Destructor(NodoA* nodo);
int NodoA_obtenerDato(NodoA* nodo);
void NodoA_modificarDato(NodoA* nodo, int nuevoDato);
NodoA* NodoA_obtenerHijoIzquierdo(NodoA* nodo);
NodoA* NodoA_obtenerHijoDerecho(NodoA* nodo);
void NodoA_modificarHijoIzquierdo(NodoA* nodo, NodoA* nuevoHijoIzquierdo);
void NodoA_modificarHijoDerecho(NodoA* nodo, NodoA* nuevoHijoDerecho);
int NodoA_esHoja(NodoA* nodo);

// Constructor por defecto
NodoA* NodoA_Constructor() {
	NodoA* nodo = (NodoA*)malloc(sizeof(NodoA));
	if (nodo != NULL) {
		nodo->hijoDerecho = NULL;
		nodo->hijoIzquierdo = NULL;
	}
	return nodo;
}

// Constructor con dato
NodoA* NodoA_ConstructorConDato(int NuevoDato) {
	NodoA* nodo = NodoA_Constructor();
	if (nodo != NULL) {
		nodo->dato = NuevoDato;
	}
	return nodo;
}

// Destructor
void NodoA_Destructor(NodoA* nodo) {
	if (nodo != NULL) {
		if (nodo->hijoIzquierdo != NULL) {
			NodoA_Destructor(nodo->hijoIzquierdo);
		}
		if (nodo->hijoDerecho != NULL) {
			NodoA_Destructor(nodo->hijoDerecho);
		}
		free(nodo);
	}
}

// Obtener dato
int NodoA_obtenerDato(NodoA* nodo) {
	if (nodo != NULL) {
		return nodo->dato;
	}
	return -1;
}

// Modificar dato
void NodoA_modificarDato(NodoA* nodo, int nuevoDato) {
	if (nodo != NULL) {
		nodo->dato = nuevoDato;
	}
}

// Obtener hijo izquierdo
NodoA* NodoA_obtenerHijoIzquierdo(NodoA* nodo) {
	if (nodo != NULL) {
		return nodo->hijoIzquierdo;
	}
	return NULL;
}

// Obtener hijo derecho
NodoA* NodoA_obtenerHijoDerecho(NodoA* nodo) {
	if (nodo != NULL) {
		return nodo->hijoDerecho;
	}
	return NULL;
}

// Modificar hijo izquierdo
void NodoA_modificarHijoIzquierdo(NodoA* nodo, NodoA* nuevoHijoIzquierdo) {
	if (nodo != NULL) {
		nodo->hijoIzquierdo = nuevoHijoIzquierdo;
	}
}

// Modificar hijo derecho
void NodoA_modificarHijoDerecho(NodoA* nodo, NodoA* nuevoHijoDerecho) {
	if (nodo != NULL) {
		nodo->hijoDerecho = nuevoHijoDerecho;
	}
}

// Verificar si es hoja
int NodoA_esHoja(NodoA* nodo) {
	if (nodo != NULL) {
		return (nodo->hijoIzquierdo == NULL && nodo->hijoDerecho == NULL);
	}
	return 0;
}

#endif /* NODO_A_H */