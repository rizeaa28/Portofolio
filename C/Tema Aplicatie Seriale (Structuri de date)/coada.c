/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "lista-generica.h"
#include "coada.h"

TCoada* InitC(void* el) //initializeaza coada
{
    TCoada* coada;
    coada = malloc(sizeof(TCoada));
    if(!coada)
        return NULL;

    coada->fata = AlocCelula(el); //aloca celula pentru element din coada
    if(!coada->fata)
        return NULL;
    coada->spate = coada->fata;

    if(el == NULL) //daca elementul cu care se initializeaza este NULL, atunci de fapt coada nu are elemente
        coada->dim = 0;
    else
        coada->dim = 1;

    return coada;
}

void IntrC(void* el, TCoada *coada) //introduce element in coada
{
    TCelula *cel;
    cel = AlocCelula(el); //aloca celula pentru elementul de introdus
    if(!cel)
        return;
    
    if(coada->fata->info != NULL) //daca coada este vida, nu trebuie mutata adresa coada->fata
    {
        coada->spate->urm = cel;
        coada->spate = cel;
    }
    else //se adauga o celula la coada
    {
        coada->fata->info = el;
        coada->spate = coada->fata;
    }
    coada->dim++;
}

void* PrimC(TCoada *coada) //copiaza primul element din coada
{
    void* info;
    info = coada->fata->info;
    return info;
}

void ExtrC(TCoada *coada) //extrage un element din coada
{    
    if(coada->dim > 1) //daca sunt mai multe elemente in coada, muta "fata" cozii
        coada->fata = coada->fata->urm;
    else //altfel elibereaza campul info
    {
        coada->fata->info == NULL;
        coada->spate = coada->fata;
    }
    coada->dim = coada->dim - 1;
}

int VidaC(TCoada coada) //test de coada vida
{
    if(coada.dim == 0)
        return 1;
    else
        return 0;
}

void EliminaC(void *el, TCoada *coada, FCmp f) //functie eliminare un element specific din coada
{
    void *cel;
    TCoada* aux;
    aux = InitC(NULL);
    if(!aux)
        return;
    
    while(!VidaC(*coada)) //parcurge coada element cu element, extrage din coada initiala, construieste o coada auxiliara
    {
        cel = PrimC(coada); 
        if(f(cel, el) == 0)
            IntrC(cel, aux);
        ExtrC(coada);
    }
    coada = aux; //reconstruieste coada initiala, dar fara elementul de eliminat
}

void AfisareC(TCoada coada, FILE* fo, TF f, char* categorie) //functie de afisare
{
    void *el;

    fprintf(fo, "Categoria %s: [", categorie);

    if(!VidaC(coada))
    { 
        while(coada.dim > 1)
        {
            el = PrimC(&coada); 
            f(el, fo);
            ExtrC(&coada);
            fprintf(fo, ", ");
        }
        
        el = PrimC(&coada);
        f(el, fo);
        ExtrC(&coada);
    }
    fprintf(fo, "].\n");
}