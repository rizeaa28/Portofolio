/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "lista-generica.h"
#include "serial.h"
#include "stiva.h"

void Citeste_input(TLista *Tend, TLista *Doc, TLista *Tut, TLista *Top, TCoada *Later, TStiva *Watching, TStiva *History, char *file_in, char *file_out)
{
    FILE *fi, *fo;
	char * line = NULL;
	size_t len = 0;

	fi = fopen(file_in, "rt"); //fisierul de input
	if (fi == NULL)
		return;
	fo = fopen(file_out, "w"); //fisierul de output

	while (getline(&line, &len, fi) != -1) //citire rand cu rand
    {
        char *functie = strtok(line, " "); //primul cuvand de pe linie e comanda ceruta

        if(strcmp(functie, "add") == 0)
        {
            TSerial *serial;
            serial = malloc(sizeof(TSerial)); //alocare structura
            if(!serial)
                continue;

            char *id_s = strtok(NULL, " ");
            serial->ID = atoi(id_s);

            serial->nume = malloc(sizeof(char)*50);
            if(!serial->nume)
                return;
            char *nume_string = strtok(NULL, " ");
            
            strcpy(serial->nume, nume_string);
            
            char* rating_s = strtok(NULL, " ");
            serial->rating = atof(rating_s);

            char *nr_sezoane_s = strtok(NULL, " ");
            serial->nr_sezoane = atoi(nr_sezoane_s);
            
            int i;
            for(i = 1; i <= serial->nr_sezoane; i++)
            {
                TSezon *p;
                p = malloc(sizeof(TSezon));

                char* nr_episoade_s = strtok(NULL, " ");
                p->nr_episoade = atoi(nr_episoade_s);

                int j;
                for(j = 1; j <= p->nr_episoade; j++)
                {
                    char* durata_s = strtok(NULL, " ");

                    if (durata_s[strlen(durata_s) - 1] == '\n')
				        durata_s[strlen(durata_s) - 1] = '\0';
			        if (durata_s[strlen(durata_s) - 1] == '\r')
				        durata_s[strlen(durata_s) - 1] = '\0';

                    TEpisod *ep;
                    ep = malloc(sizeof(TEpisod));
                    ep->durata = atoi(durata_s);

                    if(j == 1)
                        p->episoade = InitC(ep);
                    else
                        IntrC(ep, p->episoade);
                }

                if(i == 1)
                    serial->sezoane = InitC(p);
                else
                    IntrC(p, serial->sezoane);
            }
            
            int pozitie;
            pozitie = Adaugare_serial(serial, Tend, Doc, Tut);

            fprintf(fo, "Serialul %s a fost adaugat la pozitia %d.\n", serial->nume, pozitie);            
        }        

        if(strcmp(functie, "show") == 0)
        {
            char *id_show_s = strtok(NULL, " ");
            int k = 0; //verifica daca s-a afisat deja

            if (id_show_s[strlen(id_show_s) - 1] == '\n')
		        id_show_s[strlen(id_show_s) - 1] = '\0';
            if (id_show_s[strlen(id_show_s) - 1] == '\r')
		        id_show_s[strlen(id_show_s) - 1] = '\0';

            if(strcmp(id_show_s, "later") == 0)
            {
                AfisareC(*Later, fo, afiSerial, id_show_s);
                k = 1;
            }

            if(strcmp(id_show_s, "watching") == 0)
            {
                AfisareS(Watching, fo, afiSerial, id_show_s);
                k = 1;
            }

            if(strcmp(id_show_s, "history") == 0)
            {
                AfisareS(History, fo, afiSerial, id_show_s);
                k = 1;
            }

            if(strcmp(id_show_s, "top10") == 0)
            {
                Afisare_top(Top, fo, afiSerialTop);
                k = 1;
            }

            if(k == 0)
            {
                int id_show = atoi(id_show_s);
                Show(id_show, Tend, Doc, Tut, fo);
            }
            
        }
        if(strcmp(functie, "add_sez") == 0)
        {
            char* nume_show = strtok(NULL, " ");
            void* info;
            info = Find(nume_show, Tend, Doc, Tut, Top, Later, Watching);

            TSerial *show = (TSerial*)info;
            if(show->ID == 4)
            {
                TSerialTop *show = (TSerialTop*)info;
                
                show->nr_sezoane++;
                TSezon* p;
                p = malloc(sizeof(TSezon));

                char* nr_episoade_s = strtok(NULL, " ");
                p->nr_episoade = atoi(nr_episoade_s);

                int j;
                for(j = 1; j <= p->nr_episoade; j++)
                {
                char* durata_s = strtok(NULL, " ");

                if (durata_s[strlen(durata_s) - 1] == '\n')
			        durata_s[strlen(durata_s) - 1] = '\0';
		        if (durata_s[strlen(durata_s) - 1] == '\r')
			        durata_s[strlen(durata_s) - 1] = '\0';

                TEpisod *ep;
                ep = malloc(sizeof(TEpisod));
                ep->durata = atoi(durata_s);

                if(j == 1)
                    p->episoade = InitC(ep);
                else
                    IntrC(ep, p->episoade);
                }

                IntrC(p, show->sezoane);
                fprintf(fo, "Serialul %s are un sezon nou.\n", show->nume);
            }
            else
            {
                show->nr_sezoane++;
                TSezon* p;
                p = malloc(sizeof(TSezon));

                char* nr_episoade_s = strtok(NULL, " ");
                p->nr_episoade = atoi(nr_episoade_s);

                int j;
                for(j = 1; j <= p->nr_episoade; j++)
                {
                    char* durata_s = strtok(NULL, " ");

                    if (durata_s[strlen(durata_s) - 1] == '\n')
	    		        durata_s[strlen(durata_s) - 1] = '\0';
		            if (durata_s[strlen(durata_s) - 1] == '\r')
			            durata_s[strlen(durata_s) - 1] = '\0';

                    TEpisod *ep;
                    ep = malloc(sizeof(TEpisod));
                    ep->durata = atoi(durata_s);

                    if(j == 1)
                        p->episoade = InitC(ep);
                    else
                        IntrC(ep, p->episoade);
                }

                IntrC(p, show->sezoane);
                fprintf(fo, "Serialul %s are un sezon nou.\n", show->nume);
            }
        }

        if(strcmp(functie, "add_top") == 0)
        {
            TSerialTop *serial;
            serial = malloc(sizeof(TSerialTop));

            serial->ID = 4;
            
            char* poz_s = strtok(NULL, " ");
            serial->nr_ordine = atoi(poz_s);

            serial->nume = malloc(sizeof(char)*50);
            if(!serial->nume)
                return;
            char *nume_string = strtok(NULL, " ");
            
            strcpy(serial->nume, nume_string);
            
            char* rating_s = strtok(NULL, " ");
            serial->rating = atof(rating_s);

            char *nr_sezoane_s = strtok(NULL, " ");
            serial->nr_sezoane = atoi(nr_sezoane_s);
            
            int i;
            for(i = 1; i <= serial->nr_sezoane; i++)
            {
                TSezon *p;
                p = malloc(sizeof(TSezon));

                char* nr_episoade_s = strtok(NULL, " ");
                p->nr_episoade = atoi(nr_episoade_s);

                int j;
                for(j = 1; j <= p->nr_episoade; j++)
                {
                    char* durata_s = strtok(NULL, " ");

                    if (durata_s[strlen(durata_s) - 1] == '\n')
				        durata_s[strlen(durata_s) - 1] = '\0';
			        if (durata_s[strlen(durata_s) - 1] == '\r')
				        durata_s[strlen(durata_s) - 1] = '\0';

                    TEpisod *ep;
                    ep = malloc(sizeof(TEpisod));
                    ep->durata = atoi(durata_s);

                    if(j == 1)
                        p->episoade = InitC(ep);
                    else
                        IntrC(ep, p->episoade);
                }

                if(i == 1)
                    serial->sezoane= InitC(p);
                else
                    IntrC(p, serial->sezoane);
            }

            Adaugare_top(serial, Top);
            Afisare_top(Top, fo, afiSerialTop);
        }
        
        if(strcmp(functie, "later") == 0)
        {
            char* nume_show = strtok(NULL, " ");

            if (nume_show[strlen(nume_show) - 1] == '\n')
				nume_show[strlen(nume_show) - 1] = '\0';
            if (nume_show[strlen(nume_show) - 1] == '\r')
				nume_show[strlen(nume_show) - 1] = '\0';

            void *el;
            el = Find_erase(nume_show, Tend, Doc, Tut, Top);

            IntrC(el, Later);
            int poz = Later->dim;

            fprintf(fo, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n", nume_show, poz);
        }
        
        if(strcmp(functie, "watch") == 0)
        {
            char* nume_show = strtok(NULL, " ");

            char* durata_string = strtok(NULL, " ");
            if (durata_string[strlen(durata_string) - 1] == '\n')
				durata_string[strlen(durata_string) - 1] = '\0';
            if (durata_string[strlen(durata_string) - 1] == '\r')
				durata_string[strlen(durata_string) - 1] = '\0';

            int durata_watch = atoi(durata_string);

            int k;
            k = Find_to_watch(nume_show, Tend, Doc, Tut, Top, Watching, Later, History, durata_watch);
            
            if(k == 1)
                fprintf(fo, "Serialul %s a fost vizionat integral.\n", nume_show);
        }
    }
}

int main(int argv, char *argc[])
{
    //initializari
    
    TLista Tendinte = NULL;
    TLista Documentare = NULL;
    TLista Tutoriale = NULL;
    TLista Top = NULL;
    TCoada *Later;
    TStiva *Watching, *History;
    Later = InitC(NULL);
    Watching = InitS(NULL);
    History = InitS(NULL);

    //fisiere
    char *file_in, *file_out;
    file_in = argc[1];
    file_out = argc[2];

    //verificare & citire input
    if(Later != NULL && Watching != NULL && History != NULL)
        Citeste_input(&Tendinte, &Documentare, &Tutoriale, &Top, Later, Watching, History, file_in, file_out);
}