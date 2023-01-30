/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "tlg.h"

#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef void (*TF)(void*, FILE*);   /* functie afisare */
typedef int (*TFHash)(void*, int);  /* functie tabela hash */  

typedef char* (*TFVal)(void*);  /* functie extragere a campului value a unul element */
typedef char* (*TFKey)(void*);  /* functie extragere a campului key a unul element */

typedef struct /* structura tabela hash */
{
    size_t M;
    TFHash fh;
    TLDI *v;
} TH;

/* functii tabela hash */
TH* IniTH(size_t M, TFHash fh); /* initializare tabela hash */
void AfiTH(TH*a, TF afiEl, FILE* fo); /* functie afisare tabela hash (comanda "print") */
void Put_h(TH*a, void*ae, TFKey f); /* functie de inserare in tabela hash (comanda "put") */
char* Get_key(TH* h, char* key, TFVal f, TFKey fk); /* functie care intoarce key-ul unul element din tabela (comanda "get key") */
char* Remove_key(TH *h, char* key, TFKey fk); /* functie ce sterge un element din tabela hash (comanda "remove") */
void Print_bucket(TH* ah, TF afiEl, int cod, FILE* fo); /* functie de printare a unul bucket al tabelei hash */

#endif