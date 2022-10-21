/* CIUCLEA Alexia - 314CB */
/*--- functiiLG.c -- operatii de baza pentru lista dublu inlantuita generica circulara ---*/
#include "tlg.h"

int InsDupa(TLDI a, void* x)  /* inserare dupa celula cu adresa a -> 1/0 */
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula)); //alocare memorie
	if(!aux) return 0; //verificare
	
	//campul info + cele 4 legaturi
	aux->info = x; 
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;
    return 1;
}

void* Elimina(TLDI s) // elimiarea unui element din lista (refacerea a 2 legaturi)
{
	s->pre->urm = s->urm;
	s->urm->pre = s->pre;
}

TLDI InitLDI(void *x) //initializare lista
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula)); //alocare memorie
    if (!aux) return NULL; //verificare

	aux->pre = aux->urm = aux; //->pre si ->urm trebuie sa fie elementul insusi
    aux->info = x;

	return aux; //returneaza adresa primului element din lista
}
