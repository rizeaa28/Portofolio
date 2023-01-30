#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
  int *array;
  size_t used; //numarul de pozitii ocupate din array
  size_t size; //numarul de positii disponibile
} Array;

void initArray(Array *, size_t );
void insertArray(Array *, int );
void freeArray(Array *);
