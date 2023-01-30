/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "trie.h"

int CompAlph(char *str)
{
    if(*str <= 'z' && *str >= 'a')
        return (*str - 'a');

    if(*str <= 'Z' && *str >= 'A')
        return ((*str - 'A') + 26);

    if(*str <= '9' && *str >= '0')
        return ((*str - '0') + 52);
    
    if(*str == '.')
        return 62;

    if(*str == '-')
        return 63;
    
    if((*str - '&') == 1)
        return 64;

    if(*str == '?')
        return 65;
    
    if(*str == '!')
        return 66;
    
    if(*str == ' ')
        return 67;
}

struct TrieT1* newT1()
{
    struct TrieT1* node = (struct TrieT1*)malloc(sizeof(struct TrieT1)); 
    node->end = 0;
 
    for (int i = 0; i < 68; i++) //atribuie null fiecarui element din children
        node->children[i] = NULL;
    
    node->info = NULL; //informatia este un pointer spre null/ end = 0

    return node;
}

struct TrieT2* newT2()
{
    struct TrieT2* node = (struct TrieT2*)malloc(sizeof(struct TrieT2));
    node->end = 0;
 
    for (int i = 0; i < 68; i++)
        node->children[i] = NULL;
    
    node->info = NULL;

    return node;
}

void insertT1(struct TrieT1 *headT1, char* str, TBook *book)
{
    // incepe din varf
    struct TrieT1* c = headT1;
    while (*str)
    {
        // creaza un nou nod daca nu este deja creat pe calea aceea
        if (c->children[CompAlph(str)] == NULL)
            c->children[CompAlph(str)] = newT1();

        // se duce un pas mai jos in arbore
        c = c->children[CompAlph(str)];
 
        // stringul se duce la urmatorul caracter
        str++;
    }
 
    // dupa ce ajunge la sfarsitul cuvantului, retine informatia cartii si marcheaza nodul ca sfarsit de cuvant
    c->end = 1;
    c->info = book;
}

void insertT2(struct TrieT2 *headT2, char* str, TBook *book)
{
    // incepe din varf
    struct TrieT2* c = headT2;
    while (*str)
    {
        // creaza un nou nod daca nu este deja creat pe calea aceea
        if (c->children[CompAlph(str)] == NULL)
            c->children[CompAlph(str)] = newT2();

        // se duce un pas mai jos in arbore
        c = c->children[CompAlph(str)];
 
        // stringul se duce la urmatorul caracter
        str++;
    }
 
    // dupa ce ajunge la sfarsitul cuvantului, retine informatia cartii si marcheaza nodul ca sfarsit de cuvant
    c->end = 1;
    char *strtitlu = malloc(50);
    strcpy(strtitlu, book->titlu); //ne trebuie o copie a titlului
    
    if(c->info == NULL) //daca nu este creat arborele de carti al autorului il creeaza
    {
        c->info = newT1();
        insertT1(c->info, strtitlu, book); //insereaza titlul cartii in arborele de carti al autorului (triet1)
    }
    else
        insertT1(c->info, strtitlu, book);
    
}

TBook* searchT1(struct TrieT1* head, char* str)
{
    // return daca structura primita e null
    if (head == NULL)
        return NULL;


    //parcurge ca la insert, doar ca daca nu exista un nod, se opreste, nu il creeaza
    struct TrieT1* c = head;
    while (*str)
    {
        c = c->children[CompAlph(str)];
 
        if (c == NULL)
            return NULL;
        str++;
    }
 
    if(c->end == 1) //daca e sfarsit de cuvamt, returneaza pointerul spre carte
        return c->info;
    else
        return NULL;
}

void searchT2(struct TrieT2* head, char* strT1, char* strT2, FILE* fo)
{
    // return daca structura primita e null
    if (head == NULL)
        return;

    
    char *autorf = malloc(40);
    strcpy(autorf, strT2); //o copie la autor pentru afisare

    //parcurge ca la insert, doar ca daca nu exista un nod, se opreste, nu il creeaza
    struct TrieT2* c = head;
    while (*strT2)
    {
        c = c->children[CompAlph(strT2)];
 
        if (c == NULL) //daca nu exista autorul, afiseaza mesajul
        {
            fprintf(fo, "Autorul %s nu face parte din recomandarile tale.\n", autorf);
            return;
        }
        strT2++;
    }


    //daca autorul exista, cauta cartea in c->info (triet1), daca functia searcht1 returneaza null, inseamna ca acea carte nu exista
    //in caz contrar, printeaza informatia ceruta
    if(c->end == 1) 
    {
        char* titluf = malloc(50);
        strcpy(titluf, strT1);
        TBook *book = searchT1(c->info, strT1);
        if(book == NULL)
            fprintf(fo, "Cartea %s nu exista in recomandarile tale.\n", titluf);
        else
            fprintf(fo, "Informatii recomandare: %s, %s, %d, %d\n", book->titlu, book->autor, book->rating, book->nr_pag);
    }
    else
        fprintf(fo, "Autorul %s nu face parte din recomandarile tale.\n", autorf);
}

void print(struct TrieT1* head, FILE* fo)
{
    // daca nodul e sfarsit de cuvant, print titlu
    if (head->end == 1) 
        fprintf(fo, "%s\n", head->info->titlu);
  
    int i;
    for (i = 0; i < 68; i++) 
    {
        // repeta functia pentru toti children
        if (head->children[i]) 
            print(head->children[i], fo);
    }
}

int listT2(struct TrieT2* head, char* str, FILE* fo)
{
    //verifica daca exista arborele autorului ca searchT2, iar daca exista, printeaza toate cartile
    
    if (head == NULL)
        return 0;
 
    struct TrieT2* c = head;
    while (*str)
    {
        c = c->children[CompAlph(str)];
 
        if (c == NULL)
            return 0;

        str++;
    }

    if(c->end == 1)
    {
        print(c->info, fo);
        return 1;   
    }
    else
        return 0;
}

void getAuthor(struct TrieT1 *headT1, FILE *fo, int *ok)
{
    //functie auxiliara, care se plimba prin arborele de carti al unui autor si afiseaza numele autorului
    //o folosesc ca sa nu trebuiasca sa construiesc cheia (stringul) numelui autorului pentru autocomplete la listT2

    if (headT1->end == 1) 
    {  
        fprintf(fo, "%s\n", headT1->info->autor);
        (*ok) = 1;
        return;
    }
  
    int i;
    for (i = 0; i < 68 && (*ok) != 1; i++) 
    {
        if (headT1->children[i]) 
            getAuthor(headT1->children[i], fo, ok);
    }
}

void listT2_printAuthors(struct TrieT2 *headT2, int *k, FILE* fo)
{
    //functie care primeste locul in arbore ramas la sfarsitul fucntie de autocomplete
    //cauta primii 3 autori lexicografic
    //practic functioneaza ca print, doar ca am si un counter de max 3
    
    if(*k == 3)
        return;
    
    if (headT2->end == 1) 
    {
        int ok = 0;
        getAuthor(headT2->info, fo, &ok); //pentru a afla numele autorului gasit + il afiseaza
        (*k)++;
    }
  
    int i;
    for (i = 0; i < 68; i++) 
        if (headT2->children[i]) 
            listT2_printAuthors(headT2->children[i], k, fo); //repet pentru children

}

void listT2_autocomplete(struct TrieT2 *headT2, char *str, FILE* fo)
{
    if (headT2 == NULL)
        return;

    struct TrieT2* c = headT2;
    while (*str)
    {
        c = c->children[CompAlph(str)];
 
        if (c == NULL)
        {
            fprintf(fo, "Niciun autor gasit.\n");
            return;
        }
        str++;
    }
 
    //parcurg pana se termina stringul incomplet, dupa apelez functia auxiliara de mai sus
    int k = 0;
    listT2_printAuthors(c, &k, fo);
    if(k == 0) //daca k este inca 0, isneamna ca functia nu a gasit niciun autor
        fprintf(fo, "Niciun autor gasit.\n");
}

void searchT1_printtitles(struct TrieT1 *headT1, int *k, FILE* fo)
{
    //functia care primeste un subarbore de carti si printeaza primele 3 pe are le gaseste in el recursiv
    
    if(*k == 3) //maxim 3 carti
        return;
    
    if (headT1->end == 1) 
    {
        fprintf(fo, "%s\n", headT1->info->titlu); //daca gaseste o carte o afiseaza
        (*k)++;
    }
  
    int i;
    for (i = 0; i < 68; i++) 
        if (headT1->children[i]) 
            searchT1_printtitles(headT1->children[i], k, fo);
}

void searchT1_autocomplete(struct TrieT1* head, char* str, FILE *fo)
{
    //functie care cauta prin tret1 de carti daca exista carti care incep cu stringul incomplet
    //dupa apeleaza functia auxiliara de mai sus pentru a cauta primele 3 carti 
    
    if (head == NULL)
        return;
 
    struct TrieT1* c = head;
    while (*str)
    {
        c = c->children[CompAlph(str)];
 
        if (c == NULL)
        {
            fprintf(fo, "Nicio carte gasita.\n");
            return;
        }
 
        str++;
    }

    int k = 0;
    searchT1_printtitles(c, &k, fo);
}

void searchT2_autocomplete(struct TrieT2* head, char* strT1, char* strT2, FILE* fo)
{
    //la fel ca searchT2, cauta autoul, dupa aplica searchT1_autocomplete pentru a cauta cartile dupa stringul incomplet 
    // al cartii
    
    if (head == NULL)
        return;
    
    char *autorf = malloc(40);
    strcpy(autorf, strT2); //copie pentru afisare

    struct TrieT2* curr = head;
    while (*strT2)
    {
        curr = curr->children[CompAlph(strT2)];
 
        if (curr == NULL)
        {
            fprintf(fo, "Autorul %s nu face parte din recomandarile tale.\n", autorf);
            return;
        }
 
        strT2++;
    }
 
    if(curr->end == 1)
        searchT1_autocomplete(curr->info, strT1, fo);
    else
        fprintf(fo, "Autorul %s nu face parte din recomandarile tale.\n", autorf);
}


//functii care parcurg vectorul children pentru a verifica daca nodul are subarbori
int hasChildrenT1(struct TrieT1* curr)
{
    for (int i = 0; i < 68; i++)
        if (curr->children[i])
            return 1;
 
    return 0;
}

int hasChildrenT2(struct TrieT2* curr)
{
    for (int i = 0; i < 68; i++)
        if (curr->children[i])
            return 1;
 
    return 0;
}

int deletionT1(struct TrieT1 **currT1, char* str)
{
    // return daca varful arborelui este null
    if (*currT1 == NULL)
        return 0;
 
    // daca inca nu s-a ajuns la sfarsitul stringului
    if (*str)
    {
        //parcurge recursiv tot cuvantul pana la sfarsitul stringului
        //se opreste daca nodul e null, daca urmatorul nod din cuvant e null sau daca nodul e sfarsit de cuvant
        if (*currT1 != NULL && (*currT1)->children[CompAlph(str)] != NULL &&
            deletionT1(&((*currT1)->children[CompAlph(str)]), str + 1) &&
            (*currT1)->end == 0)
        {
            if (!hasChildrenT1(*currT1)) //daca nodul nu are copii (adica nu exista alte cuvinte care se continua) il sterge
            {
                free(*currT1);
                (*currT1) = NULL;
                return 1;
            }
            else
                return 0;
        }
    }
    
    // cand ajunge la sfarsitul stringului
    if (*str == '\0' && (*currT1)->end)
    {
        // daca e sfarsitu de cuvant si nu se continua, incepe sa stearga recursiv
        if (!hasChildrenT1(*currT1))
        {
            free(*currT1); 
            (*currT1) = NULL;
            return 1; //return 1 se intoarce in if de la linia 401 si semnaleaza ca se poate sterge tot cuvantul
        }
 
        // daca se continua cuvantul
        else
        {
            //nu se sterge nodul, pentru a nu se pierde cuvintede din subarbore, dar se semnaleaza ca nodul nu mai este sfarsit de cuvant
            (*currT1)->end = 0;
            return 0; //return 0 se intoarce in if de la linia 401 si semnaleaza ca nu se sterge cuvantul
        }
    }

    return 0;
}

int deletionAuthor(struct TrieT2 **currT2, char* str)
{
    if (*currT2 == NULL)
        return 0;
 
    // daca inca nu s-a ajuns la sfarsitul stringului
    if (*str)
    {
        //parcurge recursiv tot cuvantul pana la sfarsitul stringului
        //se opreste daca nodul e null, daca urmatorul nod din cuvant e null sau daca nodul e sfarsit de cuvant
        if (*currT2 != NULL && (*currT2)->children[CompAlph(str)] != NULL &&
            deletionAuthor(&((*currT2)->children[CompAlph(str)]), str + 1) &&
            (*currT2)->end == 0)
        {
            if (!hasChildrenT2(*currT2)) //daca nodul nu are copii (adica nu exista alte cuvinte care se continua) il sterge
            {
                free(*currT2);
                (*currT2) = NULL;
                return 1;
            }
            else
                return 0;
        }
    }
 
    // cand ajunge la sfarsitul stringului
    if (*str == '\0' && (*currT2)->end)
    {
        // daca e sfarsitu de cuvant si nu se continua, incepe sa stearga recursiv
        if (!hasChildrenT2(*currT2))
        {
            free(*currT2);
            (*currT2) = NULL;
            return 1;
        }
 
        // daca se continua cuvantul
        else
        {
            (*currT2)->end = 0;
            return 0;
        }
    }
 
    return 0;
}

int deletionT2(struct TrieT2 **currT2, char* strT2, char* strT1)
{
    if ((*currT2) == NULL)
        return 0;
    
    char *autorf = malloc(40);
    strcpy(autorf, strT2); //copie autor

    struct TrieT2* c = (*currT2);
    //cauta autorul in triet2
    while (*strT2)
    {
        c = c->children[CompAlph(strT2)];
 
        if (c == NULL)
            return 0;
 
        strT2++;
    }
 
    //daca il gaseste
    if(c->end == 1)
    {
        //delete la cartea din subarborele lui de carti
        deletionT1(&(c->info), strT1);
        
        if(c->info != NULL) //daca subarborele a ramas gol dupa delete il sterge
        {
            if(!hasChildrenT1(c->info))
            {
                c->info = NULL;
                c->end = 0;
                deletionAuthor(currT2, autorf); //delete la autor daca nu mai are carti deloc
                return 1;
            }
        }
        else
        {
            c->end = 0;
            deletionAuthor(currT2, autorf); //delete la autor daca nu mai are carti deloc
            return 1;
        }

        return 1; //nu da delete la autor daca tot are carti
    }
    else
        return 0;
}