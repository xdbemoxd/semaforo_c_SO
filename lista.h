#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "avion.h"
#include "equipaje.h"
#include "vuelo.h"
#include "nodo.h"
#ifndef lista_h
#define lista_h

// Definir diferentes tipos de listas
typedef struct{
	int longitud;
	equipaje *prim;
} listaEquipaje, ListaEquipaje;

typedef struct{
	int longitud;
	avion *prim;
} listaAvion, ListaAvion;

typedef struct{
	int longitud;
	vuelo *prim;
} listaVuelo, ListaVuelo;

typedef struct{
	int longitud;
	nodo *prim;
} lista, Lista;

int longitud_L(lista *L){
	 return L->longitud;
	
}

void crear_L(lista *L){
	L->prim = NULL;
	L->longitud = 0;
}

int buscar_L(lista L, int V){
	nodo *aux;
	aux = L.prim;
	if(L.longitud > 0){
		while(aux->info != V && aux->prox != NULL){
			aux = aux->prox;
			}
		if(aux->prox == NULL){
			return 0;
			} else {
			return 1;	
			}
		} else {
		return 0;	
		}
}

int consultar_L(lista L, int P){
	int valor, i;
	nodo *aux;
	if (L.longitud >= 1) {
		aux = L.prim;
		for(i = 2; i <= P; i++){
			aux = aux->prox;
		}
		valor = aux->info;
	} else {
	valor = -1;
	}
	return(valor);
}

void insertar_inicio_L(lista *L, int Val){
	nodo *nuevo,*aux;
	nuevo = (nodo*)malloc(sizeof(nodo));
	nuevo->info = Val;
	if (L->longitud == 0){
		nuevo->prox = NULL;
	} else {
		aux = L->prim;
		nuevo->prox = aux;
	}
	L->prim = nuevo;
	L->longitud = L->longitud + 1;
}

void insertar_final_L(lista *L, int Val){
	nodo *nuevo,*aux;
	int i;
	nuevo = (nodo*)malloc(sizeof(nodo));
	nuevo->info = Val;
	nuevo->prox = NULL;
	if (L->longitud == 0){
		L->prim = nuevo;
	} else {
		aux = L->prim;
		for(i = 2; i <= L->longitud; i++){
			aux = aux->prox;
		}
		aux->prox = nuevo;
	}
	L->longitud = L->longitud + 1;
}

void insertar_posicion_L(lista *L, int Val, int pos){
	nodo *nuevo,*aux,*prev;
	int i;
	if (pos <= L->longitud + 1 && pos >= 1){
		if (pos == 1){
			insertar_inicio_L(L,Val);
		} else if(pos == L->longitud + 1){
			insertar_final_L(L,Val);
		} else {
			nuevo = (nodo*)malloc(sizeof(nodo));
			nuevo->info = Val;
			aux = L->prim;
			for(i = 2;i <= pos; i++){
				prev = aux;
				aux = aux->prox;
			}
			nuevo->prox = aux;
			prev->prox = nuevo;
			L->longitud = L->longitud + 1;
		}
	}
}

void eliminar_posicion_L(lista *L, int pos){
	nodo *aux,*prev;
	int i;
	if (pos <= L->longitud && pos >= 1){
		if (pos == 1){
			aux = L->prim;
			L->prim = aux->prox;
			free(aux);
		} else {
			aux = L->prim;
			for(i = 2;i <= pos; i++){
				prev = aux;
				aux = aux->prox;
			}
			prev->prox = aux->prox;
			free(aux);
		}
		L->longitud = L->longitud - 1;
	}
}



int es_vacia_L(lista L){
	int vacia;
	if(L.prim == NULL){
		vacia = 1;
	} else {
		vacia = 0;
	}
	return(vacia);
}

void eliminar_elemento_L(lista *L, int elem){
	nodo *aux,*prev;
	int i, encontrado;
	encontrado = 0;
	prev = NULL;
	if (L->longitud > 0){
		aux = L->prim;
		while(aux != NULL && encontrado == 0){
			if (elem == aux->info){
				if(prev == NULL){
					L->prim = aux->prox;
				} else {
					prev->prox = aux->prox;
				}
				encontrado = 1;
				free(aux);
			} else {
				prev = aux;
				aux = aux->prox;
			}
		}
		if(encontrado != 0){
			L->longitud = L->longitud - 1;
		}
	}
}

void vaciar_L(lista *L){
	while(L->longitud > 0){
		eliminar_posicion_L(L,1);
	}
}

void invertir_L(lista *L, lista L2){
	int i,j, mitadL, val_a, val_b;
	j = L->longitud;
	mitadL = floor(L->longitud / 2);
	for (i = 1; i <= mitadL; i++){
		val_a = consultar_L(L2,i);
		val_b = consultar_L(L2,j);
		eliminar_posicion_L(L,i);
		insertar_posicion_L(L,val_b,i);
		eliminar_posicion_L(L,j);
		insertar_posicion_L(L,val_a,j);
		j = j - 1;
	}
}

// Funciones para listaEquipaje
void crear_LE(listaEquipaje *L){
	L->prim = NULL;
	L->longitud = 0;
}

void insertar_inicio_LE(listaEquipaje *L, equipaje *Val){
	equipaje *aux;
	if (L->longitud == 0){
		Val->prox = NULL;
	} else {
		aux = L->prim;
		Val->prox = aux;
	}
	L->prim = Val;
	L->longitud = L->longitud + 1;
}

void insertar_final_LE(listaEquipaje *L, equipaje *Val){
	equipaje *aux;
	int i;
	Val->prox = NULL;
	if (L->longitud == 0){
		L->prim = Val;
	} else {
		aux = L->prim;
		for(i = 2; i <= L->longitud; i++){
			aux = aux->prox;
		}
		aux->prox = Val;
	}
	L->longitud = L->longitud + 1;
}

// Funciones para listaAvion
void crear_LA(listaAvion *L){
	L->prim = NULL;
	L->longitud = 0;
}

void insertar_inicio_LA(listaAvion *L, avion *Val){
	avion *aux;
	if (L->longitud == 0){
		Val->prox = NULL;
	} else {
		aux = L->prim;
		Val->prox = aux;
	}
	L->prim = Val;
	L->longitud = L->longitud + 1;
}

void insertar_final_LA(listaAvion *L, avion *Val){
	avion *aux;
	int i;
	Val->prox = NULL;
	if (L->longitud == 0){
		L->prim = Val;
	} else {
		aux = L->prim;
		for(i = 2; i <= L->longitud; i++){
			aux = aux->prox;
		}
		aux->prox = Val;
	}
	L->longitud = L->longitud + 1;
}

// Funciones para listaVuelo
void crear_LV(listaVuelo *L){
	L->prim = NULL;
	L->longitud = 0;
}

void insertar_inicio_LV(listaVuelo *L, vuelo *Val){
	vuelo *aux;
	if (L->longitud == 0){
		Val->prox = NULL;
	} else {
		aux = L->prim;
		Val->prox = aux;
	}
	L->prim = Val;
	L->longitud = L->longitud + 1;
}

void insertar_final_LV(listaVuelo *L, vuelo *Val){
	vuelo *aux;
	int i;
	Val->prox = NULL;
	if (L->longitud == 0){
		L->prim = Val;
	} else {
		aux = L->prim;
		for(i = 2; i <= L->longitud; i++){
			aux = aux->prox;
		}
		aux->prox = Val;
	}
	L->longitud = L->longitud + 1;
}

// Funciones de búsqueda por ID
equipaje* buscar_equipaje_por_id(listaEquipaje L, int id){
	equipaje *aux = L.prim;
	while(aux != NULL){
		if(aux->id == id) return aux;
		aux = aux->prox;
	}
	return NULL;
}

avion* buscar_avion_por_id(listaAvion L, int id){
	avion *aux = L.prim;
	while(aux != NULL){
		if(aux->id == id) return aux;
		aux = aux->prox;
	}
	return NULL;
}

vuelo* buscar_vuelo_por_id(listaVuelo L, int id){
	vuelo *aux = L.prim;
	while(aux != NULL){
		if(aux->id == id) return aux;
		aux = aux->prox;
	}
	return NULL;
}

// Funciones para eliminar elementos
void eliminar_equipaje_por_id(listaEquipaje *L, int id){
	equipaje *aux = L->prim;
	equipaje *prev = NULL;
	
	while(aux != NULL && aux->id != id){
		prev = aux;
		aux = aux->prox;
	}
	
	if(aux != NULL){
		if(prev == NULL){
			L->prim = aux->prox;
		} else {
			prev->prox = aux->prox;
		}
		free(aux);
		L->longitud--;
	}
}

void eliminar_avion_por_id(listaAvion *L, int id){
	avion *aux = L->prim;
	avion *prev = NULL;
	
	while(aux != NULL && aux->id != id){
		prev = aux;
		aux = aux->prox;
	}
	
	if(aux != NULL){
		if(prev == NULL){
			L->prim = aux->prox;
		} else {
			prev->prox = aux->prox;
		}
		free(aux);
		L->longitud--;
	}
}

void eliminar_vuelo_por_id(listaVuelo *L, int id){
	vuelo *aux = L->prim;
	vuelo *prev = NULL;
	
	while(aux != NULL && aux->id != id){
		prev = aux;
		aux = aux->prox;
	}
	
	if(aux != NULL){
		if(prev == NULL){
			L->prim = aux->prox;
		} else {
			prev->prox = aux->prox;
		}
		free(aux);
		L->longitud--;
	}
}

// Funciones para vaciar listas
void vaciar_LE(listaEquipaje *L){
	equipaje *actual = L->prim;
	equipaje *siguiente;
	
	while(actual != NULL){
		siguiente = actual->prox;
		free(actual);
		actual = siguiente;
	}
	L->prim = NULL;
	L->longitud = 0;
}

void vaciar_LA(listaAvion *L){
	avion *actual = L->prim;
	avion *siguiente;
	
	while(actual != NULL){
		siguiente = actual->prox;
		free(actual);
		actual = siguiente;
	}
	L->prim = NULL;
	L->longitud = 0;
}

void vaciar_LV(listaVuelo *L){
	vuelo *actual = L->prim;
	vuelo *siguiente;
	
	while(actual != NULL){
		siguiente = actual->prox;
		free(actual);
		actual = siguiente;
	}
	L->prim = NULL;
	L->longitud = 0;
}

#endif
