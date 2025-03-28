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
void NodoA_Destructor(NodoA* this);
int NodoA_obtenerDato(NodoA* this);
void NodoA_modificarDato(NodoA* this, int nuevoDato);
NodoA* NodoA_obtenerHijoIzquierdo(NodoA* this);
NodoA* NodoA_obtenerHijoDerecho(NodoA* this);
void NodoA_modificarHijoIzquierdo(NodoA* this, NodoA* nuevoHijoIzquierdo);
void NodoA_modificarHijoDerecho(NodoA* this, NodoA* nuevoHijoDerecho);
int NodoA_esHoja(NodoA* this);

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
void NodoA_Destructor(NodoA* this) {
	if (this != NULL) {
		if (this->hijoIzquierdo != NULL) {
			NodoA_Destructor(this->hijoIzquierdo);
		}
		if (this->hijoDerecho != NULL) {
			NodoA_Destructor(this->hijoDerecho);
		}
		free(this);
	}
}

// Obtener dato
int NodoA_obtenerDato(NodoA* this) {
	if (this != NULL) {
		return this->dato;
	}
	return -1;
}

// Modificar dato
void NodoA_modificarDato(NodoA* this, int nuevoDato) {
	if (this != NULL) {
		this->dato = nuevoDato;
	}
}

// Obtener hijo izquierdo
NodoA* NodoA_obtenerHijoIzquierdo(NodoA* this) {
	if (this != NULL) {
		return this->hijoIzquierdo;
	}
	return NULL;
}

// Obtener hijo derecho
NodoA* NodoA_obtenerHijoDerecho(NodoA* this) {
	if (this != NULL) {
		return this->hijoDerecho;
	}
	return NULL;
}

// Modificar hijo izquierdo
void NodoA_modificarHijoIzquierdo(NodoA* this, NodoA* nuevoHijoIzquierdo) {
	if (this != NULL) {
		this->hijoIzquierdo = nuevoHijoIzquierdo;
	}
}

// Modificar hijo derecho
void NodoA_modificarHijoDerecho(NodoA* this, NodoA* nuevoHijoDerecho) {
	if (this != NULL) {
		this->hijoDerecho = nuevoHijoDerecho;
	}
}

// Verificar si es hoja
int NodoA_esHoja(NodoA* this) {
	if (this != NULL) {
		return (this->hijoIzquierdo == NULL && this->hijoDerecho == NULL);
	}
	return 0;
}

#endif /* NODO_A_H */