/* CIUCLEA Alexia - 314CB */
#include "stiva.h"
#include "lista-generica.h"

TStiva* InitS(void *el) //initializare stiva    
{ 
	  TStiva* a = (TStiva*)malloc(sizeof (TStiva));
  	if (!a) 
		return NULL;

  	if(el == NULL) //daca initializam cu null, de fapt stiva nu are elemente, deci dim = 0;
      a->dime = 0;
    else
      a->dime = 1;   

  	a->baza = AlocCelula(el); //aloca o celula de tip lista ptr baza si varf
    if(!a->baza)
        return 0;

    a->varf = a->baza;
  	return a; //returneaza adresa stivei
}

void Push(void *el, TStiva *stiva)
{
  TCelula *cel;
  cel = AlocCelula(el); //aloca o celula ptr elementul de inserat
  if(!cel)
    return;
  
  if(stiva->dime == 0) //daca stiva este goala nu trebuie schimbata adresa, ci doar *info
  {
    stiva->varf->info = el;
    stiva->baza = stiva->varf;
    stiva->dime = 1;
  }
  else  //adauga o celula si muta varful stivei
  {
    cel->urm = stiva->varf;
    stiva->varf = cel;
    stiva->dime++;
  }
}

void* Prim(TStiva stiva) //returneaza primul element din stiva
{
  void *info;
  info = stiva.varf->info;
  return info;
}

void Pop(TStiva *stiva) //elimina varful stivei
{
  if(stiva->dime == 1) //daca este doar un element, trebuie doar schimbat *info
  {
    stiva->varf->info = NULL;
    stiva->baza = stiva->varf;
    stiva->dime = 0;
  }
  else //se muta varful stivei cu un pas
  {
    stiva->varf = stiva->varf->urm;
    stiva->dime = stiva->dime - 1;
  }
}

void Rastoarna(TStiva* stiva, TStiva* aux) //rastoarna stiva din "aux" in "stiva"
{
  void *el;
  while(aux->dime > 0)
  {
    el = aux->varf->info;
    Push(el, stiva);
    Pop(aux);
  }
}

void EliminaS(void* el, TStiva* stiva, FCmp f) //elimina un anumit element din stiva cu ajutorul unei functii de comparare
{
  void* cel;
  TStiva* aux;
  aux = InitS(NULL);
  if(!aux)
    return;

  //se pune elementele stivei initiale in aux in afara de elementul de eliminat

  while(stiva->dime > 0)
  {
    cel = Prim(*stiva);
    if(f(cel, el) == 0)
      Push(cel, aux);
    Pop(stiva);
  }
  Rastoarna(stiva, aux); //reconstruirea stivei initiale acum cu un element eliminat
}

void* Avans(TStiva *stiva) //functie avans pas stiva
{
  void *el;
  Pop(stiva);
  el = Prim(*stiva);
  return el;
}

void AfisareS(TStiva* stiva, FILE* fo, TF f, char* categorie) //functie de afisare a stivei
{
    void* el;
    fprintf(fo, "Categoria %s: [", categorie);
    TStiva *aux;
    aux = InitS(NULL);
    if(!aux)
      return;

    if(stiva->dime > 0) //daca stiva are elemente
    {
      while(stiva->dime > 1)
      {
        el = Prim(*stiva);
        f(el, fo); //print element
        Push(el, aux); 
        Pop(stiva);
        fprintf(fo, ", ");
      }

      el = Prim(*stiva);
      f(el, fo);
      Push(el, aux);
      Pop(stiva);

      Rastoarna(stiva, aux);
    }
    
    fprintf(fo, "].\n");
}
