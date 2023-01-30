#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "array.h"

// Initializeaza un array
void initArray(Array *a, size_t init) {
    a->array = malloc(init * sizeof(int));
    a->used = 0;
    a->size = init;
}

// Insereaza element in array
void insertArray(Array *a, int e) {
    if (a->used == a->size) {
        a->size += 20;
        a->array = realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used] = e;
    a->used++;
}

// Sterge si da free la memorie
void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}