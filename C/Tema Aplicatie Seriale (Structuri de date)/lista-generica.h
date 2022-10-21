/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#ifndef _LISTA_SIMPLU_INLANTUITA_
#define _LISTA_SIMPLU_INLANTUITA_

typedef struct celula
{ void* info;
  struct celula * urm;
} TCelula, *TLista; /* tipurile Celula, Lista */

typedef int (*FCmp)(void*, void*); //functie de comparare 2 elemente
TLista AlocCelula(void*); //functie alocare celula lista
int Adaugare_lista(void*, TLista*, FCmp); //adaugare element in lista in in functie de o functie de comparare
typedef void (*TF)(void*, FILE*); //functie de afisare element lista
void Elimina(TLista*, TLista); //functie eliminare element din lista

#endif