/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "lista-generica.h"
#include "coada.h"
#include "stiva.h"

#ifndef SERIAL

typedef struct{
    int ID, nr_sezoane;
    char *nume;
    float rating;
    TCoada *sezoane;
}TSerial;

typedef struct{
    int ID, nr_sezoane, nr_ordine;
    char *nume;
    float rating;
    TCoada *sezoane;
}TSerialTop;

typedef struct{
    TCoada *episoade;
    int nr_episoade;
}TSezon;

typedef struct{
    int durata;
}TEpisod;

//functii de comparare intre seriale
int Comparare_seriale(void*, void*); 
int Comparare_top(void*, void*);
int Comp(void*, void*);

//functii de afisare a structurii
void afiSerial(void*, FILE*);
void afiSerialTop(void*, FILE*);

//functii de afisare a listelor
void Afisare(TLista*, TF, FILE*, int);
void Show(int, TLista*, TLista*, TLista*, FILE*);
void Afisare_top(TLista*, FILE*, TF);

//functii adaugare in liste
int Adaugare_serial(void*, TLista*, TLista*, TLista*);
void Adaugare_top(void*, TLista*);

//Functii de cautare elemente in liste/cozi/stive
void* Find(char*, TLista*, TLista*, TLista*, TLista*, TCoada*, TStiva*);
TSerialTop* Find_top(char*, TLista*);
void* Find_erase(char*, TLista*, TLista*, TLista*, TLista*);
int Find_to_watch(char*, TLista*, TLista*, TLista*, TLista*, TStiva*, TCoada*, TStiva*, int);

int Watch(void*, TStiva*, TStiva*, int); //functie pentru comanda "watch"

#endif