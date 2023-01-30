/* CIUCLEA Alexia - 314CB */
/*  functii-tabHash.c - functii tabela hash */

#include "thash.h"

TH* IniTH(size_t M, TFHash fh) //initializare tabela hash
{
    TH* h = (TH *) calloc(sizeof(TH), 1); //alocare memorie tabela hash
    if (!h) //verificare alocare memorie
    {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLDI *) calloc(M, sizeof(TLDI)); //alocare vector de liste
    if(!h->v) //verificare
    {
        printf("eroare alocare vector de pointeri TLDI in hash\n");
        free(h);
        return NULL;
    }

    h->M = M; //numarul de bucket-uri
    h->fh = fh; //functia de hash-uri
    return h;
}

void AfiTH(TH* ah, TF afiEl, FILE* fo) //functie afisare tabela hash (comanda "print")
{
    TLDI p, el;
    int i;
    for(i = 0; i < ah->M; i++) //parcurgere vector de liste
    {
        p = ah->v[i];
        if(p) //daca exista lista, parcurge acea lista si afiseaza elementele
        {
            fprintf(fo, "%d: ",i);
            afiEl(p->info, fo);
            for(el = p->urm; el != p; el = el->urm)
                afiEl(el->info, fo);
            fprintf(fo, "\n");
        }
    }
}

void Print_bucket(TH* ah, TF afiEl, int cod, FILE* fo) //functie de printare a unul bucket al tabelei hash
{
    TLDI p, el;
    p = ah->v[cod]; //accesam lista ceruta in comanda print_bucket
    if(p) 
    {
        afiEl(p->info, fo);
        for(el = p->urm; el != p; el = el->urm) //parcurge lista si o afiseaza
            afiEl(el->info, fo);
        fprintf(fo, "\n");
    }
    else
        fprintf(fo, "VIDA\n"); //daca nu exista lista afiseaza ca e vida
}

char* Get_key(TH *h, char* key, TFVal f, TFKey fk) //functie care intoarce key-ul unul element din tabela (comanda "get key")
{
    TLDI p, el;
    int i;
    for(i = 0; i < h->M; i++) //parcurgere vector de liste
    {
        p = h->v[i];
        if(p) //daca exista lista, o parcurge pentru a verifica daca exista key-ul cerut
        {
            if(strcmp(fk(p->info), key) == 0)
                return f(p->info); //daca gaseste key-ul il returneaza
            for(el = p->urm; el != p; el = el->urm)
                if(strcmp(fk(el->info), key) == 0)
                    return f(el->info); //daca gaseste key-ul il returneaza
        }
    }
    return NULL; //daca nu gaseste key-ul returneaza NULL
}

char* Remove_key(TH *h, char* key, TFKey fk) //functie ce sterge un element din tabela hash (comanda "remove")
{
    TLDI p, el;
    int i;
    for(i = 0; i < h->M; i++) //parcurgere vector de liste
    {
        p = h->v[i];
        if(p)
        {
            if(strcmp(fk(p->info), key) == 0) //daca primul element din lista trebuie sters
            {
                if(p->urm == p) //daca e singurul element din lista, elementul vectorului de liste devine NULL din nou
                {
                    h->v[i] = NULL;
                    free(p);
                }
                else //altfel se muta inceputul listei
                {
                    h->v[i] = p->urm;
                    Elimina(p);
                    break;
                }
            }
            for(el = p->urm; el != p; el = el->urm) //daca nu e primul element din lista
                if(strcmp(fk(el->info), key) == 0)
                {
                    Elimina(el);
                    break;
                }
            }
        }
    return NULL;
}

void Put_h(TH*a, void* ae, TFKey f)
{
    int cod = a->fh(ae, a->M); //lista din vectorul de liste in care trebuie inserat elementul
    TLDI el;
    int ok = 0;

    if(a->v[cod] == NULL)
        a->v[cod] = InitLDI(ae); //daca lista nu exista, o initializeaza
    else //daca exista
    {
        if(strcmp(f((a->v[cod])->info), ae) >= 0) //daca trebuie inserat inainte de primul element se muta inceputul listei
        {
            InsDupa((a->v[cod])->pre, ae);
            a->v[cod] = (a->v[cod])->pre;
            ok = 1;
        }
        else
            for(el = a->v[cod]->urm; el != a->v[cod] && !ok; el = el->urm) //parcurge lista
                if(strcmp(f(el->info), f(ae)) >= 0) //insereaza elementul in ordine lexicografica
                {
                    InsDupa(el->pre, ae);
                    ok = 1;
                    break;
                }
        if(ok == 0) //daca elementul nu a fost inserat pana acum, inseamna ca trebuie inserat la sfarsitul listei
            InsDupa((a->v[cod])->pre, ae);
    }
}
