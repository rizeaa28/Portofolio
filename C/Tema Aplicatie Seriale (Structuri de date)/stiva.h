/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "lista-generica.h"

#ifndef STIVA
#define STIVA

typedef struct
{ 
	TCelula *baza, *varf;
	int dime;
} TStiva;

TStiva* InitS(void*); //initializeaza stiva
void Push(void*, TStiva*); //insereaza un element in stiva
void* Prim(TStiva); //copiaza elementul din varf
void Pop(TStiva*); //scoate elementul din varf
void EliminaS(void*, TStiva*, FCmp); //elimina un anumit element din stiva
void AfisareS(TStiva*, FILE*, TF, char*); //afiseaza stiva
void* Avans(TStiva*); //functie avans pas stiva 




#endif