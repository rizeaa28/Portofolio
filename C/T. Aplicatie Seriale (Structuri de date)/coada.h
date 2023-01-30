/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "lista-generica.h"

typedef struct{
    TCelula *fata, *spate;
    int dim;
}TCoada;

TCoada* InitC(void*); //initializare coada
void IntrC(void*, TCoada*); //introduce un element in coada
void ExtrC(TCoada*); //extrage un element din coada
void* PrimC(TCoada*); //copiaza elementul din fata cozii
void AfisareC(TCoada, FILE*, TF, char*); //afisare coada
void EliminaC(void*, TCoada*, FCmp); //elimina un element specific din coada


