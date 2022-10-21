/* CIUCLEA Alexia - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct{
    char *titlu, *autor;
    int rating, nr_pag;
}TBook;

struct TrieT1
{
    struct TrieT1 *children[68];
    int end; //este 1 daca este sfarsitul cuvantului
    TBook* info;
};

struct TrieT2
{
    struct TrieT2 *children[68];
    int end; //este 1 daca este sfarsitul cuvantului
    struct TrieT1 *info;
};

//Functii de alocare memorie pentru triet1 respectiv triet2
struct TrieT1* newT1();
struct TrieT2* newT2();

//Functii de inserare informatie in triet1 respectiv triet2
void insertT1(struct TrieT1*, char*, TBook*);
void insertT2(struct TrieT2*, char*, TBook*);

//Functii de cautare a unei informatii in trie
TBook* searchT1(struct TrieT1*, char*);
void searchT2(struct TrieT2*, char*, char*, FILE*);

//Functia de listare a cartilor unui autor
int listT2(struct TrieT2*, char*, FILE*);

//Corespondentul functiilor de mai sus, dar in cazul in care este nevoie de autocomplete
void listT2_autocomplete(struct TrieT2*, char*, FILE*);
void searchT1_autocomplete(struct TrieT1*, char*, FILE*);
void searchT2_autocomplete(struct TrieT2*, char*, char*, FILE*);

//Functii de stergere a unei informatii din triet1/triet2
int deletionT1(struct TrieT1**, char*);
int deletionT2(struct TrieT2**, char*, char*);