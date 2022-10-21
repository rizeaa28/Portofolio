/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "serial.h"

int Comparare_seriale(void *av, void *bv) //compara ratingul a doua seriale pentru inserare in lista
{
    TSerial *a = (TSerial*)av;
    TSerial *b = (TSerial*)bv;

    if(a->rating > b->rating)
        return 1;
    if(a->rating < b->rating)
        return 2;
    if(strcmp(a->nume, b->nume) < 0)
        return 1;
    return 2;
}

int Comparare_top(void *av, void *bv) //acelasi lucru ca mai sus dar pentru seriale de tip Top
{
    TSerialTop *a = (TSerialTop*)av;
    TSerialTop *b = (TSerialTop*)bv;

    if(a->nr_ordine > b->nr_ordine)
        return 2;
    else
        return 1;
}

int Adaugare_serial(void *serial_v, TLista *Tend, TLista *Doc, TLista *Tut) //adauga serial clasic in lista
{
    int pozitie_serial;
    TSerial* serial = (TSerial *)serial_v;

    //adauga in lista in functie de ID si intoarce pozitia

    if(serial->ID == 1)
    {
        pozitie_serial = Adaugare_lista(serial_v, Tend, Comparare_seriale);
    }
    
    if(serial->ID == 2)
    {
        pozitie_serial = Adaugare_lista(serial_v, Doc, Comparare_seriale);
    }

    if(serial->ID == 3)
    {
        pozitie_serial = Adaugare_lista(serial_v, Tut, Comparare_seriale);
    }

    return pozitie_serial;
}

void afiSerialTop(void *el, FILE* f) //functie afisare serial de tip top
{
    TSerialTop *serial = (TSerialTop *)el;
    fprintf(f, "(%s, %.1f)", serial->nume, serial->rating);
}

void afiSerial(void *el, FILE* f) //functie de afisare serial de tip clasic
{
    TSerial *serial = (TSerial *)el;

    if(serial->ID == 4) //daca serialul de fapt este de tip top, apeleaza functia specifica
        afiSerialTop(el, f);
    else
        fprintf(f, "(%s, %.1f)", serial->nume, serial->rating);
}

void Afisare(TLista *l, TF f, FILE* fout, int id) //afisarea unei lista
{
    fprintf(fout, "Categoria %d: [", id); //categoria

    TLista p;
    p = *l;

    if(p != NULL)
    {
        for(; p->urm != NULL; p = p->urm) //parcurgere lista
        {
            f(p->info, fout); //print
            fprintf(fout, ", ");
        }
        f(p->info, fout);
    }
    fprintf(fout, "].\n");
}

void Show(int id, TLista *Ten, TLista *Doc, TLista *Tut, FILE* fo)//afisare in functie de ID
{    
    if(id == 1)
        Afisare(Ten, afiSerial, fo, id);
    if(id == 2)
        Afisare(Doc, afiSerial, fo, id);
    if(id == 3)
        Afisare(Tut, afiSerial, fo, id);
}

TSerialTop* Find_top(char* nume_show, TLista* Top) //cauta un serial in lista Top
{
    TLista p;
    p = *Top;

    for(; p != NULL; p = p->urm) //parcurgere lista
    {
        TSerialTop* serial = (TSerialTop*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
            return serial; //returneaza adresa serialului
    }
}

void Decrementare(TLista* Top, char* nume_show)
{
    //functie care decrementeaza pozitia in lista top a elementelor de dupa un element eliminat

    TLista p;
    p = *Top;
    int k = 0;
    for(; p != NULL; p = p->urm)  //parcurgere
    {
        TSerialTop* serial = (TSerialTop*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0) //daca am gasti elementul inserat, incepem sa decremenetam
            k = 1;

        if(k == 1)
            serial->nr_ordine = serial->nr_ordine - 1;
    }
}

void* Find_erase(char *nume_show, TLista *Ten, TLista *Doc, TLista *Tut, TLista* Top)
{
    //functie care gaseste si elimina din lista un element, dupa il returneaza

    TLista p, s, aux;
    int gasit = 0;
    
    p = *Ten; aux = NULL; //pentru lista Tendinte
    for(; p != NULL && !gasit; aux = p, p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            gasit = 1;
            s = p;
            Elimina(Ten, aux);
        }
    }

    p = *Doc; aux = NULL; //pentru lista Documentare
    for(; p != NULL && !gasit; aux = p, p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            gasit = 1;
            s = p;
            Elimina(Doc, aux);
        }
    }

    p = *Tut; aux = NULL; //pentru lista Tutoriale
    for(; p != NULL && !gasit; aux = p, p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            gasit = 1;
            s = p;
            Elimina(Tut, aux);
        }
    }

    p = *Top; aux = NULL; //pentru lista Top
    for(; p != NULL && !gasit; aux = p, p = p->urm)
    {
        TSerialTop* serial = (TSerialTop*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            gasit = 1;
            s = p;
            Decrementare(Top, nume_show);
            Elimina(Top, aux);
        }
    }
    return (s->info);
}

void* FindS(TStiva stiva, char* nume_show) //cauta in stiva
{
    void* el;

    while(stiva.dime >= 1) //parcurgere stiva
    {
        el = Prim(stiva);
        TSerial *serial = (TSerial*)el;
        if(serial->ID != 4) //daca nu este serial de tip Top
        {
            if(strcmp(serial->nume, nume_show) == 0) //gasire element
                return el;
        }
        else //daca este serial de tip Top
        {
            TSerialTop* serialt = (TSerialTop*)el;
            if(strcmp(serialt->nume, nume_show) == 0) //gasire element
                return el;
        }
        
        if(stiva.dime > 1)//pas
        {
            el = Avans(&stiva);
        }
        else
            stiva.dime = stiva.dime - 1;
    }
    return NULL;
}

void* FindC(TCoada coada, char* nume_show) //functie cautare in coada
{
    void* el;

    while(coada.dim > 0) //parcurgere coada
    {
        el = PrimC(&coada); 
        TSerial *serial = (TSerial*)el;
        if(serial->ID != 4) //daca nu este serial de tip Top
        {
            if(strcmp(serial->nume, nume_show) == 0) //gasire element
                return el;
        }
        else //daca este de tip Top
        {
            TSerialTop* serialt = (TSerialTop*)el;
            if(strcmp(serialt->nume, nume_show) == 0) //gasire element
                return el;
        }
        ExtrC(&coada); //pas
    }
    return NULL;
}


int Comp(void* el1, void* el2) //functie comparare (egalitate) intre 2 seriale
{
    TSerial* s1 = (TSerial*)el1; //cast la amandoua serialele
    TSerial* s2 = (TSerial*)el2;

    if(s1->ID == 4 && s2->ID == 4) //daca amandoua serialele sunt de tip top
    {
        TSerialTop* st1 = (TSerialTop*)el1;
        TSerialTop* st2 = (TSerialTop*)el2;
        if(strcmp(st1->nume, st2->nume) == 0)
            return 1;
        else
            return 0;
    }

    if(s1->ID == 4 || s2->ID == 4) //daca doar unul este
        return 0;

    if(strcmp(s1->nume, s2->nume) == 0) //in rest serialele sunt de tip clasic si se poate face compararea
        return 1;
    else
        return 0;
}

void Pozitii_top(TLista *Top) //reface pozitiile din lista Top in cazul inserarii
{
    TLista p;
    p = *Top;
    int nr = 1;

    for(; p != NULL; p = p->urm) //parcurgere lista
    {
        TSerialTop *serial = (TSerialTop*)(p->info);
        serial->nr_ordine = nr;
        nr++;
        if(nr > 10)
            p->urm = NULL; //maxim 10 elemente in lista Top
    }
}

void Adaugare_top(void* serial, TLista* Top) //adaugare in lista top
{    
    int pozitie;
    pozitie = Adaugare_lista(serial, Top, Comparare_top); //adaugare in lista efectiva
    Pozitii_top(Top); //refacerea pozitiilor
}

void Afisare_top(TLista* Top, FILE* fo, TF f) //afisare lista top
{
    fprintf(fo, "Categoria top10: [");

    TLista p;
    p = *Top;
    int nr = 0;

    if(p != NULL)
    {
       
        for(; p->urm != NULL && nr < 9; p = p->urm) //parcurgere + maxim 10 elemente
        {
            TSerialTop *serial = (TSerialTop*)(p->info); //cast serial
            f(p->info, fo); //afisare
            fprintf(fo, ", ");
            nr++;
        }
        f(p->info, fo); 
    }
    fprintf(fo, "].\n");
}

int Watch_top(void* el, TStiva *Watching, TStiva* History, int durata_watch) //functia Watch dar pentru serial de tip Top
{
    //functia Watch este explicata pe lung mai jos, Watch_top este acelasi lucru dar pentru structura specifica
    TSerialTop* serial = (TSerialTop*)el;
    int terminat = 0;

    while (serial->sezoane->dim != 0 && !terminat)
    {
        void* sezon_void;
        sezon_void = PrimC(serial->sezoane);
        TSezon *sezon = (TSezon*)sezon_void;

        while(sezon->episoade->dim != 0 && !terminat)
        {
            void *episod_void;
            episod_void = PrimC(sezon->episoade);
            TEpisod *episod = (TEpisod*)episod_void;

            durata_watch = durata_watch - episod->durata;

            if(durata_watch < 0)
            {
                episod->durata = 0 - durata_watch;
                terminat = 1;
                IntrC(episod, sezon->episoade);
            }

            if(durata_watch == 0)
                terminat = 1;

            ExtrC(sezon->episoade);
        }

        if(terminat)
        {
            if(sezon->episoade->dim != 0)
                IntrC(sezon, serial->sezoane);
        }
        
        ExtrC(serial->sezoane);
    }
    
    if(terminat)
    {
        if(serial->sezoane->dim == 0)
        {
            Push(serial, History);
            return 1;
        }
        else
        {
            Push(serial, Watching);
            return 0;
        }
    }
    else
    {
        Push(serial, History);
        return 1;
    }
}

int Watch(void* el, TStiva *Watching, TStiva* History, int durata_watch) //comanda watch
{
    TSerial* serial = (TSerial*)el;
    int terminat = 0;

    if(serial->ID == 4) //daca serialul este de tip top, apeleaza functia Watch_top
    {
        int k = Watch_top(el, Watching, History, durata_watch);
        return k;
    }

    while (serial->sezoane->dim != 0 && !terminat) //parcurge
    {
        void* sezon_void;
        sezon_void = PrimC(serial->sezoane);
        TSezon *sezon = (TSezon*)sezon_void;
        
        while(sezon->episoade->dim != 0 && !terminat) //parcurgere sezoane
        {
            void *episod_void;
            episod_void = PrimC(sezon->episoade);
            TEpisod *episod = (TEpisod*)episod_void;

            durata_watch = durata_watch - episod->durata; //scade durata episodului din cea de vizionare
            
            if(durata_watch < 0) //daca s-a vizionat mai mult decat se "putea"
            {
                episod->durata = 0 - durata_watch; 
                terminat = 1;
                IntrC(episod, sezon->episoade); //se reintroduce episodul in lista, dar cu durata schimbata (restul la care trebuie sa te uiti)
            }

            if(durata_watch == 0) //daca s-a terminat episodul "la fix"
                terminat = 1;

            ExtrC(sezon->episoade); //pas parcurgere
        }

        if(terminat) //daca s-a terminat durata de vizionare
        {
            if(sezon->episoade->dim != 0)
                IntrC(sezon, serial->sezoane); //punem inapoi in lista rstul de sezoane nevizionate
        }
        ExtrC(serial->sezoane); //pas
    }
    
    if(terminat)
    {
        if(serial->sezoane->dim == 0) //daca am terminat timpul de vizionare dar si serialul
        {
            Push(serial, History); //insereaza serialul in history
            return 1;
        }
        else //daca am temrinat timpul de vizionare dar nu serialul
        {
            Push(serial, Watching); //insereaza serialul in watching
            return 0;
        }
    }
    else //daca am terminat serialul dar nu timpuld e vizionare
    {
        Push(serial, History); //insereaza serialul in history
        return 1;
    }
}


int Find_to_watch(char *nume_show, TLista *Ten, TLista *Doc, TLista *Tut, TLista* Top, TStiva *Watching, TCoada *Later, TStiva *History, int durata)
{
    //functie care cauta ce serial trebuie sa fie vizionat, returneaza daca a fost vizionat integral serialul

    TLista p, aux;
    void *el;

    el = FindS(*Watching, nume_show); //cautare in stiva Wathcing
    if(el != NULL)
    {
        EliminaS(el, Watching, Comp); //elimina el din stiva
        int k;
        k = Watch(el, Watching, History, durata); //comanda watch
        return k;
    }

    el = FindC(*Later, nume_show); //cautare in coada
    if(el != NULL)
    {
        EliminaC(el, Later, Comp); //elimina din coada
        int k = 0;
        k = Watch(el, Watching, History, durata);
        return k;
    }

    //parcurgere lista ca in Find_erase, se adauga apelul functiei watch in cazul gasirii

    p = *Ten; aux = NULL;
    for(; p != NULL; aux = p, p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            Elimina(Ten, aux);

            void* element;
            element = (p->info);
            int k;
            k = Watch(element, Watching, History, durata);
            return k;
        }
    }

    p = *Doc; aux = NULL;
    for(; p != NULL; aux = p, p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            Elimina(Doc, aux);
            void* element;
            element = (p->info);
            int k;
            k = Watch(element, Watching, History, durata);
            return k;
        }
    }

    p = *Tut;
    for(; p != NULL; aux = p, p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            Elimina(Tut, aux);
            void* element;
            element = (p->info);
            int k;
            k = Watch(element, Watching, History, durata);
            return k;
        }
    }

    p = *Top;
    for(; p != NULL; aux = p, p = p->urm)
    {
        TSerialTop* serial = (TSerialTop*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
        {
            Decrementare(Top, nume_show);
            Elimina(Top, aux);
            void* element;
            element = (p->info);
            int k;
            k = Watch(element, Watching, History, durata);
            return k;
        }
    }
    return 0;
}


void* Find(char *nume_show, TLista *Ten, TLista *Doc, TLista *Tut, TLista* Top, TCoada* Later, TStiva* Watching)
{
    //functie de cautare serial pentru adaugare sezon
    
    TLista p;
    
    //Cautare prin elemente ca in functia Fin_erase explicata mai sus, dar fara eliminare

    p = *Ten;
    for(; p != NULL; p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
            return (p->info);
    }

    p = *Doc;
    for(; p != NULL; p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
            return (p->info);
    }

    p = *Tut;
    for(; p != NULL; p = p->urm)
    {
        TSerial* serial = (TSerial*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
            return (p->info);
    }

    p = *Top;
    for(; p != NULL; p = p->urm)
    {
        TSerialTop* serial = (TSerialTop*)(p->info);
        if(strcmp(serial->nume, nume_show) == 0)
            return (p->info);
    }

    //de aici cauta in Stiva Watching si in coada later
    void *el;

    el = FindC(*Later, nume_show); //cautare in Later
    if(el != NULL)
        return el;

    el = FindS(*Watching, nume_show); //cautare in Watching
    if(el != NULL)
        return el; 
        
    return NULL;
}
