/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "trie.h"

TBook* BuildBook(char *titlu, char *autor, char *rating_s, char *nr_pag_s)
{
    TBook *book = (TBook*) malloc(sizeof(TBook)); //aloca memorie pentru carte

    book->titlu = malloc(50*sizeof(char)); //aloca memorie ptr titlu
    strcpy(book->titlu, titlu);

    book->autor = malloc(40*sizeof(char)); //aloca memorie ptr autor
    strcpy(book->autor, autor);

    book->rating = atoi(rating_s);

    if (nr_pag_s[strlen(nr_pag_s) - 1] == '\n')
		nr_pag_s[strlen(nr_pag_s) - 1] = '\0';
	if (nr_pag_s[strlen(nr_pag_s) - 1] == '\r')
		nr_pag_s[strlen(nr_pag_s) - 1] = '\0';

    book->nr_pag = atoi(nr_pag_s);

    return book;
}

void Citeste_input(struct TrieT1 *headT1, struct TrieT2 *headT2, char *file_in, char *file_out)
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
        
        if(strcmp(functie, "add_book") == 0)
        {
            char *titlu, *autor, *rating_s, *nr_pag_s;
            titlu = strtok(NULL, ":");
            autor = strtok(NULL, ":");
            rating_s = strtok(NULL, ":");
            nr_pag_s = strtok(NULL, ":");

            char *titluf = malloc(50); //copii autor si titlu pentru inserare
            char *autorf = malloc(40);

            strcpy(titluf, titlu);
            strcpy(autorf, autor);

            //contruieste cartea cu informatiile citite
            TBook *new_book = BuildBook(titlu, autor, rating_s, nr_pag_s);

            //insereaza cartea in cei doi arbori
            insertT1(headT1, titluf, new_book);
            insertT2(headT2, autorf, new_book);
        }
        
        if(strcmp(functie, "search_book") == 0)
        {
            char *titlu;
            titlu = strtok(NULL, ":");

            if (titlu[strlen(titlu) - 1] == '\n')
		        titlu[strlen(titlu) - 1] = '\0';
	        if (titlu[strlen(titlu) - 1] == '\r')
		        titlu[strlen(titlu) - 1] = '\0';

            if(titlu[strlen(titlu) - 1] == '~') //varianta autocomplete
            {
                titlu[strlen(titlu) - 1] = '\0';
                searchT1_autocomplete(headT1, titlu, fo);
            }
            else //varianta clasica
            {
                char *titluf = malloc(50);
                strcpy(titluf, titlu);

                TBook *found_book;
                found_book = searchT1(headT1, titlu); //cauta cartea in arborele de carti

                if(found_book != NULL) //daca o gaseste, afiseaza informatiile
                    fprintf(fo, "Informatii recomandare: %s, %s, %d, %d\n", found_book->titlu, found_book->autor, found_book->rating, found_book->nr_pag);
                else //daca nu o gaseste
                    fprintf(fo, "Cartea %s nu exista in recomandarile tale.\n", titluf);
            }
            
        }
        
        if(strcmp(functie, "list_author") == 0)
        {
            char *autor;
            autor = strtok(NULL, ":");

            if (autor[strlen(autor) - 1] == '\n')
		        autor[strlen(autor) - 1] = '\0';
	        if (autor[strlen(autor) - 1] == '\r')
		        autor[strlen(autor) - 1] = '\0';

            if(autor[strlen(autor) - 1] == '~') //varianta autocomplete
            {
                autor[strlen(autor) - 1] = '\0';
                listT2_autocomplete(headT2, autor, fo);
            }
            else //varianta clasica
            {
                char *autorf = malloc(50);
                strcpy(autorf, autor);

                int ok = listT2(headT2, autorf, fo); //functia listT2 afiseaza de asemenea informatia, doar intoarce 0 daca nu s-a afisat nimic

                if(ok == 0)
                    fprintf(fo, "Autorul %s nu face parte din recomandarile tale.\n", autor);
            }
            
        }

        if(strcmp(functie, "search_by_author") == 0)
        {
            char *autor;
            autor = strtok(NULL, ":");

            if (autor[strlen(autor) - 1] == '\n')
		        autor[strlen(autor) - 1] = '\0';
	        if (autor[strlen(autor) - 1] == '\r')
		        autor[strlen(autor) - 1] = '\0';

            if(autor[strlen(autor) - 1] == '~') //varianta autocomplete pentru autor
            {
                autor[strlen(autor) - 1] = '\0';
                listT2_autocomplete(headT2, autor, fo);
            }
            else //varianta clasica ptr autor
            {
                char *titlu;
                titlu = strtok(NULL, ":");

                if (titlu[strlen(titlu) - 1] == '\n')
		            titlu[strlen(titlu) - 1] = '\0';
	            if (titlu[strlen(titlu) - 1] == '\r')
		            titlu[strlen(titlu) - 1] = '\0';

                if(titlu[strlen(titlu) - 1] == '~') //varianta autocomplete pentru titlu
                {
                    titlu[strlen(titlu) - 1] = '\0';
                    searchT2_autocomplete(headT2, titlu, autor, fo);
                }
                else //varianta clasica
                    searchT2(headT2, titlu, autor, fo);
            }
        }

        if(strcmp(functie, "delete_book") == 0)
        {
            char *titlu;
            titlu = strtok(NULL, ":");

            if (titlu[strlen(titlu) - 1] == '\n')
		        titlu[strlen(titlu) - 1] = '\0';
	        if (titlu[strlen(titlu) - 1] == '\r')
		        titlu[strlen(titlu) - 1] = '\0';

            char *titluf = malloc(50);
            strcpy(titluf, titlu);

            TBook *book;
            book = searchT1(headT1, titlu); //cauta cartea in arborele de carti
            if(book != NULL) //daca o gaseste
            {
                deletionT1(&headT1, titluf); //o sterge din arborele de carti

                char *titlucpy = malloc(50);
                strcpy(titlucpy, book->titlu);

                char *autorcpy = malloc(40);
                strcpy(autorcpy, book->autor);
                deletionT2(&headT2, autorcpy, titlucpy); //o sterge din arborele de autori si din arborele de carti al autorului
            }
            else
                fprintf(fo, "Cartea %s nu exista in recomandarile tale.\n", titluf); //daca nu exista in arborele de carti
        }
    }
}

int main(int argv, char *argc[])
{
    struct TrieT1* headT1; //trieul de carti
    headT1 = newT1();

    struct TrieT2* headT2; //trieul de autori
    headT2 = newT2();

    //fisiere
    char *file_in, *file_out;
    file_in = argc[1];
    file_out = argc[2];
    Citeste_input(headT1, headT2, file_in, file_out);
}
