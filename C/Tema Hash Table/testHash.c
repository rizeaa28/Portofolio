/* CIUCLEA Alexia - 314CB */
/*-- testHash.c  */
#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>
#include <stdlib.h>


typedef struct {
	char key[50];
	char value[30];
} TSite; //structura (key-value)

int codHash(void *a, int M) //functie hash a tabelei
{
	TSite * site = (TSite *) a;
	char * key = site->key;
	
	int suma = 0;

	for(int i = 0; key[i] != '\0'; i++)
		suma = suma + (int)key[i]; //suma literelor campului key

	return suma % M;
}

void afisareSite(void * element, FILE* fo) //functie de afisare a campului value
{
	TSite * s = (TSite *) element;
	fprintf(fo, "%s ", s->value);
}

char* Value(void* p) //functie de extragere a campului value al unul element
{
    TSite *el = (TSite*) p;

	return el->value;    
}

char* Key(void* p) //functie de extragere a campului key al unul element
{
    TSite *el = (TSite*) p;

	return el->key;    
}

int Citeste_Input(TH *h, char *file_in, char *file_out) //functie care citeste inputul din file-ul respectiv
{
	FILE *fi, *fo;
	char * line = NULL;
	size_t len = 0;

	fi = fopen(file_in, "rt"); //fisierul de input
	if (fi == NULL)
		return 0;
	fo = fopen(file_out, "w"); //fisierul de output

	while (getline(&line, &len, fi) != -1) //citire rand cu rand
	{
		char *functie = strtok(line, " "); //primul cuvand de pe linie e comanda ceruta

		if(strcmp(functie, "put") == 0) //comanda put
		{
			TSite *p;
			p = malloc(sizeof(TSite));

			char* key = strtok(NULL," ");
			char* value = strtok(NULL," ");

			if (value[strlen(value) - 1] == '\n')
				value[strlen(value) - 1] = '\0';
			if (value[strlen(value) - 1] == '\r')
				value[strlen(value) - 1] = '\0';
			
			strcpy(p->key, key);
			strcpy(p->value, value); //obtinem din input campurile key si value

			if(!Get_key(h, key, Value, Key)) //daca elementul nu este deja inserat, il inseram
				Put_h(h, (void*) p, Key);
		}

		if(strcmp(functie, "get") == 0) //comanda get key
		{
			char* key = strtok(NULL, " ");
			char* value;

			if (key[strlen(key) - 1] == '\n')
				key[strlen(key) - 1] = '\0';
			if (key[strlen(key) - 1] == '\r')
				key[strlen(key) - 1] = '\0';

			value = Get_key(h, key, Value, Key); //obtine value

			if(value) //daca Nu e NULL
				fprintf(fo,"%s\n", value);
			else
				fprintf(fo,"NULL\n");
		}

		if(strcmp(functie, "remove") == 0) //comanda remove
		{
			char* key = strtok(NULL, " ");

			if (key[strlen(key) - 1] == '\n')
				key[strlen(key) - 1] = '\0';
			if (key[strlen(key) - 1] == '\r')
				key[strlen(key) - 1] = '\0';

			if(Get_key(h, key, Value, Key)) //daca exista elementul, ii da remove din tabela
				Remove_key(h, key, Key);
		}

		if(strcmp(functie, "find") == 0) //comanda find
		{
			char* key = strtok(NULL, " ");

			if (key[strlen(key) - 1] == '\n')
				key[strlen(key) - 1] = '\0';
			if (key[strlen(key) - 1] == '\r')
				key[strlen(key) - 1] = '\0';

			char* value;
			value = Get_key(h, key, Value, Key); //foloseste comanda get_key ca sa afle daca exista un value pentru acel key in tabela

			if(value) //daca exista
				fprintf(fo,"True\n");
			else //daca nu exista
				fprintf(fo,"False\n");
		}

		if (functie[strlen(functie) - 1] == '\n')
			functie[strlen(functie) - 1] = '\0';
		if (functie[strlen(functie) - 1] == '\r')
			functie[strlen(functie) - 1] = '\0';

		if(strcmp(functie, "print") == 0) //comanda print
		{
			AfiTH(h, afisareSite, fo); //afiseaza tabela
		}

		if(strcmp(functie, "print_bucket") == 0) //comanda print_bucket
		{
			char* cod = strtok(NULL, " ");
			int cod_int = atoi(cod);

			if(cod_int < h->M) //daca exista bucket-ul, il afiseaza
				Print_bucket(h, afisareSite, cod_int, fo);
		}
	}
}

int main(int argv, char *argc[])
{
	int M;
	char *file_in, *file_out;

	M = atoi(argc[1]); //numarul de bucket-uri
	file_in = argc[2]; //fisierul input
	file_out = argc[3]; //fisierul output
	
	// Initializare Tabela Hash
	TH * h = NULL;
	h = (TH *) IniTH(M, codHash);
	if (h == NULL)
	{
		printf("Tabela hash nu a putut fi generata\n");
        return 0;
	}

	Citeste_Input(h, file_in, file_out); //citirea inputului

	return 0;
}
