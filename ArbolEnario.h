#ifndef ARBOL_ENARIO_H
#define ARBOL_ENARIO_H

#include <stdio.h>
#include <stdlib.h>
#include "NodoA.h"
#include "lista.h"
#include "equipaje.h"

// Estructura para el árbol
typedef struct 
{
	NodoA* raiz;
} ArbolEnario;

typedef struct ArbolEnarioAux
{
	int catidadAviones;
	NodoArbolAvion* primero; 
}ArbolEnarioAux;

void arbolEnarioConstructorAviones(ArbolEnarioAux *arbol2) 
{
	ArbolEnarioAux* arbol = ( ArbolEnarioAux* ) malloc ( sizeof( ArbolEnarioAux ) );
	if (arbol != NULL) {
		arbol->primero = NULL;
		arbol->catidadAviones = 0;
	}
	arbol2 = arbol;
}


void insertarAvion( ArbolEnarioAux *arbol, int numAvion )
{
	NodoArbolAvion* nodoAux = ( NodoArbolAvion* ) malloc ( sizeof( NodoArbolAvion ) );
	
	nodoAux->numAvion = numAvion;
	
	if ( arbol->primero == NULL )
	{
		arbol->catidadAviones++;
		arbol->primero = nodoAux;
		nodoAux->hermano = NULL;
	}
	else
	{
		NodoArbolAvion* nodoAux2 = ( NodoArbolAvion* ) malloc ( sizeof( NodoArbolAvion ) );

		nodoAux2 = arbol->primero;

		nodoAux->hermano = nodoAux2;

		arbol->primero = nodoAux;
	}
	
}

void mostrarAvionesArbol( ArbolEnarioAux arbol )
{
	NodoArbolAvion* nodoAux = ( NodoArbolAvion* ) malloc ( sizeof( NodoArbolAvion ) );

	nodoAux = arbol.primero;

	printf("te voy a mostrar los aviones guardados\n");

	while ( nodoAux != NULL )
	{
		printf( "%d\n", nodoAux->numAvion );
		nodoAux = nodoAux->hermano;
	}
	
}

void insertarHijo( ArbolEnarioAux *arbol, listaEquipaje* listaEqui, int numAvion, int idMaleta )
{
	NodoArbolAvion* nodoAuxArbol = ( NodoArbolAvion* ) malloc ( sizeof( NodoArbolAvion ) );
	equipaje* nodoAuxEqui = ( equipaje* ) malloc ( sizeof( equipaje ) );
	ApuntadorEquipaje* nodoAuxApuntEqui = ( ApuntadorEquipaje* ) malloc ( sizeof( ApuntadorEquipaje ) );
	

	nodoAuxArbol = arbol->primero;

	while ( nodoAuxArbol->numAvion != numAvion )
	{
		nodoAuxArbol = nodoAuxArbol->hermano;
	}

	nodoAuxEqui = listaEqui->prim;

	while ( nodoAuxEqui->id != idMaleta )
	{
		nodoAuxEqui = nodoAuxEqui->prox;
	}

	if ( nodoAuxArbol->primero == NULL )
	{
		nodoAuxApuntEqui->siguiente = NULL;
		nodoAuxApuntEqui->senalaEqui = nodoAuxEqui;
		nodoAuxArbol->primero = nodoAuxApuntEqui;
	}
	else
	{
		ApuntadorEquipaje* nodoAuxApuntEqui2 = ( ApuntadorEquipaje* ) malloc ( sizeof( ApuntadorEquipaje ) );

		nodoAuxApuntEqui2 = nodoAuxArbol->primero;

		nodoAuxApuntEqui->senalaEqui = nodoAuxEqui;
		nodoAuxApuntEqui->siguiente = nodoAuxApuntEqui2;
		nodoAuxArbol->primero = nodoAuxApuntEqui;
	}
	
}

int existeAvion( ArbolEnarioAux* arbol, int numAvion )
{
	NodoArbolAvion* nodoAuxArbol = ( NodoArbolAvion* ) malloc ( sizeof( NodoArbolAvion ) );
	int aux=1;

	if ( arbol->primero != NULL )
	{
		nodoAuxArbol = arbol->primero;

		while ( nodoAuxArbol != NULL )
		{
			if ( nodoAuxArbol->numAvion == numAvion )
			{
				break;
			}

			nodoAuxArbol = nodoAuxArbol->hermano;
			
		}

		if (nodoAuxArbol != NULL)
		{
			aux = 0;
		}
	}

	return aux;
}

void mostrarEquipajesAvion( ArbolEnarioAux arbol, int numAvion )
{
	NodoArbolAvion* nodoAuxArbol = ( NodoArbolAvion* ) malloc ( sizeof( NodoArbolAvion ) );
	ApuntadorEquipaje* nodoAuxApuntEqui = ( ApuntadorEquipaje* ) malloc ( sizeof( ApuntadorEquipaje ) );
	equipaje* nodoAuxEqui = ( equipaje* ) malloc ( sizeof( equipaje ) );

	nodoAuxArbol = arbol.primero;

	while ( nodoAuxArbol != NULL )
	{
		if ( nodoAuxArbol->numAvion == numAvion )
		{
			break;
		}

		nodoAuxArbol = nodoAuxArbol->hermano;
			
	}

	if ( nodoAuxArbol != NULL )
	{
		nodoAuxApuntEqui = nodoAuxArbol->primero;

		printf("\n\n\nel avion %d, llevo los siguientes equipajes\n\n\n",numAvion);

		while ( nodoAuxApuntEqui != NULL )
		{
			nodoAuxEqui = nodoAuxApuntEqui->senalaEqui;
			mostrar_equipaje( nodoAuxEqui );
			nodoAuxApuntEqui = nodoAuxApuntEqui->siguiente;
		}
		
	}
	
}

// Funciones auxiliares internas
void ArbolEnario_buscarPadreAux(NodoA* actual, int padre, int hijo, int* band);
void ArbolEnario_agregarHijoAux(NodoA* actual, int hijo);
void ArbolEnario_recorridoInOrdenAux(NodoA* actual, lista* lista, int padre);
void ArbolEnario_recorridoPreOrdenAux(NodoA* actual, lista* lista);
void ArbolEnario_recorridoPostOrdenAux(NodoA* actual, lista* lista);
void ArbolEnario_recorridoLevelOrdenAux(NodoA* actual, lista* lista);
void ArbolEnario_recorridoInOrdenDerechaAux(NodoA* actual, lista* lista, int padre);
int ArbolEnario_elementoRepetido(int elemento, lista* lista);
void ArbolEnario_obtenerHijos(NodoA* padre, NodoA** hijos, int* numHijos);

// Constructor
ArbolEnario* ArbolEnario_Constructor() {
	ArbolEnario* arbol = (ArbolEnario*)malloc(sizeof(ArbolEnario));
	if (arbol != NULL) {
		arbol->raiz = NULL;
	}
	return arbol;
}

// Constructor con dato inicial
ArbolEnario* ArbolEnario_ConstructorConDato(int dato) {
	ArbolEnario* arbol = ArbolEnario_Constructor();
	if (arbol != NULL) {
		arbol->raiz = NodoA_ConstructorConDato(dato);
	}
	return arbol;
}

// Destructor
void ArbolEnario_Destructor(ArbolEnario* this) {
	if (this != NULL) {
		if (this->raiz != NULL) {
			NodoA_Destructor(this->raiz);
		}
		free(this);
	}
}

// Verificar si es semilla (solo tiene raíz)
int ArbolEnario_esSemilla(ArbolEnario* this) {
	if (this != NULL && this->raiz != NULL) {
		return NodoA_esHoja(this->raiz);
	}
	return 1;
}

// Buscar padre (función auxiliar)
void ArbolEnario_buscarPadreAux(NodoA* actual, int padre, int hijo, int* band) {
	if (actual != NULL && *band == 0) {
		if (NodoA_obtenerDato(actual) == padre) {
			*band = 1;
			ArbolEnario_agregarHijoAux(actual, hijo);
		} else {
			ArbolEnario_buscarPadreAux(NodoA_obtenerHijoIzquierdo(actual), padre, hijo, band);
			if (*band == 0) {
				ArbolEnario_buscarPadreAux(NodoA_obtenerHijoDerecho(actual), padre, hijo, band);
			}
		}
	}
}

// Agregar hijo auxiliar
void ArbolEnario_agregarHijoAux(NodoA* actual, int hijo) {
	NodoA* nuevo = NodoA_ConstructorConDato(hijo);
	if (NodoA_obtenerHijoIzquierdo(actual) == NULL) {
		NodoA_modificarHijoIzquierdo(actual, nuevo);
	} else {
		NodoA* temp = NodoA_obtenerHijoIzquierdo(actual);
		while (NodoA_obtenerHijoDerecho(temp) != NULL) {
			temp = NodoA_obtenerHijoDerecho(temp);
		}
		NodoA_modificarHijoDerecho(temp, nuevo);
	}
}

// Agregar hijo (función pública)
void ArbolEnario_agregarHijo(ArbolEnario* this, int padre, int hijo) {
	if (this != NULL && this->raiz != NULL) {
		int band = 0;
		if (NodoA_obtenerDato(this->raiz) == padre) {
			ArbolEnario_agregarHijoAux(this->raiz, hijo);
		} else {
			ArbolEnario_buscarPadreAux(this->raiz, padre, hijo, &band);
		}
	}
}

// Obtener dato de la raíz
int ArbolEnario_obtenerDatoRaiz(ArbolEnario* this) {
	if (this != NULL && this->raiz != NULL) {
		return NodoA_obtenerDato(this->raiz);
	}
	return -1;
}

// Recorrido InOrden
void ArbolEnario_recorridoInOrdenAux(NodoA* actual, lista* lista, int padre) {
	if (actual != NULL) {
		ArbolEnario_recorridoInOrdenAux(NodoA_obtenerHijoIzquierdo(actual), lista, NodoA_obtenerDato(actual));
		insertar_final_L(lista, NodoA_obtenerDato(actual));
		if (NodoA_obtenerHijoDerecho(actual) != NULL) {
			ArbolEnario_recorridoInOrdenAux(NodoA_obtenerHijoDerecho(actual), lista, padre);
		}
	}
}

// InOrden (función pública)
void ArbolEnario_inOrden(ArbolEnario* this, lista* lista) {
	if (this != NULL && this->raiz != NULL) {
		crear_L(lista);
		ArbolEnario_recorridoInOrdenAux(this->raiz, lista, -1);
	}
}

// Recorrido PreOrden
void ArbolEnario_recorridoPreOrdenAux(NodoA* actual, lista* lista) {
	if (actual != NULL) {
		insertar_final_L(lista, NodoA_obtenerDato(actual));
		if (NodoA_obtenerHijoIzquierdo(actual) != NULL) {
			ArbolEnario_recorridoPreOrdenAux(NodoA_obtenerHijoIzquierdo(actual), lista);
		}
		if (NodoA_obtenerHijoDerecho(actual) != NULL) {
			ArbolEnario_recorridoPreOrdenAux(NodoA_obtenerHijoDerecho(actual), lista);
		}
	}
}

// PreOrden (función pública)
void ArbolEnario_preOrden(ArbolEnario* this, lista* lista) {
	if (this != NULL && this->raiz != NULL) {
		crear_L(lista);
		ArbolEnario_recorridoPreOrdenAux(this->raiz, lista);
	}
}

// Recorrido PostOrden
void ArbolEnario_recorridoPostOrdenAux(NodoA* actual, lista* lista) {
	if (actual != NULL) {
		if (NodoA_obtenerHijoIzquierdo(actual) != NULL) {
			ArbolEnario_recorridoPostOrdenAux(NodoA_obtenerHijoIzquierdo(actual), lista);
		}
		if (NodoA_obtenerHijoDerecho(actual) != NULL) {
			ArbolEnario_recorridoPostOrdenAux(NodoA_obtenerHijoDerecho(actual), lista);
		}
		insertar_final_L(lista, NodoA_obtenerDato(actual));
	}
}

// PostOrden (función pública)
void ArbolEnario_postOrden(ArbolEnario* this, lista* lista) {
	if (this != NULL && this->raiz != NULL) {
		crear_L(lista);
		ArbolEnario_recorridoPostOrdenAux(this->raiz, lista);
	}
}

// Recorrido por niveles
void ArbolEnario_recorridoLevelOrdenAux(NodoA* actual, lista* lista) {
	if (actual == NULL) return;
	
	// Implementación usando una cola simple
	NodoA* cola[1000];
	int frente = 0, final = 0;
	
	cola[final++] = actual;
	
	while (frente < final) {
		NodoA* nodo = cola[frente++];
		insertar_final_L(lista, NodoA_obtenerDato(nodo));
		
		if (NodoA_obtenerHijoIzquierdo(nodo) != NULL) {
			cola[final++] = NodoA_obtenerHijoIzquierdo(nodo);
		}
		NodoA* hermano = NodoA_obtenerHijoIzquierdo(nodo);
		while (hermano != NULL) {
			if (NodoA_obtenerHijoDerecho(hermano) != NULL) {
				cola[final++] = NodoA_obtenerHijoDerecho(hermano);
			}
			hermano = NodoA_obtenerHijoDerecho(hermano);
		}
	}
}

// LevelOrden (función pública)
void ArbolEnario_levelOrden(ArbolEnario* this, lista* lista) {
	if (this != NULL && this->raiz != NULL) {
		crear_L(lista);
		ArbolEnario_recorridoLevelOrdenAux(this->raiz, lista);
	}
}

// Elemento repetido
int ArbolEnario_elementoRepetido(int elemento, lista* lista) {
	return buscar_L(*lista, elemento);
}

// Obtener hijos de un nodo
void ArbolEnario_obtenerHijos(NodoA* padre, NodoA** hijos, int* numHijos) {
	*numHijos = 0;
	if (padre != NULL) {
		NodoA* hijo = NodoA_obtenerHijoIzquierdo(padre);
		while (hijo != NULL) {
			hijos[*numHijos] = hijo;
			(*numHijos)++;
			hijo = NodoA_obtenerHijoDerecho(hijo);
		}
	}
}

// Mostrar árbol
void ArbolEnario_mostrarArbol(ArbolEnario* this) {
	if (this == NULL || this->raiz == NULL) return;

	lista listaNivel;
	crear_L(&listaNivel);
	
	printf("Árbol en nivel por nivel: ");
	ArbolEnario_levelOrden(this, &listaNivel);
	
	nodo* actual = listaNivel.prim;
	while (actual != NULL) {
		printf("%d ", actual->info);
		actual = actual->prox;
	}
	printf("\n");
	
	vaciar_L(&listaNivel);
}

#endif /* ARBOL_ENARIO_H */