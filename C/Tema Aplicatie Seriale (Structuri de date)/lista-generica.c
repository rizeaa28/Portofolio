/* CIUCLEA Alexia - 314CB */
#include <ctype.h>
#include "lista-generica.h"

TLista AlocCelula(void* e)          /* adresa celulei create sau NULL */
{ TLista aux = (TLista)malloc(sizeof(TCelula));  /* incearca alocare */
  if (aux)                            /* aux contine adresa noii celule */
  { aux->info = e;                   /* completeaza noua celula */
    aux->urm = NULL;
  }
  return aux;             /* rezultatul este adresa noii celule sau NULL */
}

int Adaugare_lista(void *serialAdd, TLista *l, FCmp Comparare_seriale)
{
  int pozitie = 1;
  if(*l == NULL) //daca lista este vida
  {
    TLista p;
    p = AlocCelula(serialAdd);
    if(!p)
      return 0;
    *l = p; //primul sis ingurul element este cel adaugat
    return 1;
  }
  else
  {
    TLista p, aux = NULL;
    p = *l;
    for(; p != NULL; p = p->urm)
    {
      if(Comparare_seriale(serialAdd, p->info) == 1) //daca elementul trebuie inserat aici
      {
        TLista ser;
        ser = AlocCelula(serialAdd);
        if(!ser)
          return 0;
            
        if(p == *l) //daca trebuie inserat inainte de primul element, se schimba adresa de inceput a listei
        {
          ser->urm = *l;
          *l = ser;
          return 1;
        }
        else //altfel
        {
          ser->urm = p;
          aux->urm = ser;
          return pozitie;
        }
      }
      aux = p; //elementul din urma celui cu care se compara, pentru stabilirea legaturilor 
      pozitie = pozitie + 1;
    }
    TLista ser;
    ser = AlocCelula(serialAdd);
    if(!ser)
      return 0;
    aux->urm = ser; //inserare la sfarsit
    return pozitie;
  }
}

void Elimina(TLista *l, TLista aux)
{
  if(aux == NULL)
    (*l) = (*l)->urm; //daca trebuie eliminat primul element
  
  TLista p = *l;

  for(; p != NULL; p = p->urm)
    if(p == aux)
      p->urm = p->urm->urm; //reface legatura pentru a elimina un element
}
