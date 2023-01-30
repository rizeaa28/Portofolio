/* CIUCLEA Alexia - 314CB */
/*-- tlg.h --- LISTA DUBLU INLANTUITA GENERICA CIRCULARA---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celula
{
  void *info;              /* adresa element extern */
  struct celula *pre, *urm;  /* legaturi spre celulele vecine */
} TCelula, *TLDI;

typedef void (*TF)(void*, FILE*);     /* functie afisare/eliberare un element */

/* Initializare lista vida; info santinela = NULL */
TLDI InitLDI();

/* Inserare dupa celula cu adresa p -> 1/0. Preconditia: spatiul pentru
   elementul cu adresa x a fost alocat si actualizat la nivelul aplicatiei */
int InsDupa(TLDI p, void* x);

//Functie elimiare element din lista generica
void *Elimina(TLDI s);
#endif
