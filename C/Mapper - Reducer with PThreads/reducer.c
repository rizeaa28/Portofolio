#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "reducer.h"

// compara 2 int-uri pentru sortare
int compare (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void *reducer_function(void* arg) {
    
    // structura primita ca argument
    struct Reducer_arg *r_arg = (struct Reducer_arg*)arg;

    // reducer id (ne trebuie pentru a sti ce putere extragem)
    int reducer_id = r_arg->thread_id - r_arg->mappers;
    
    // lista cu numerele extrase
    Array output;
    initArray(&output, 1);

    // asteapta sa se termine mapperi pentru a incepe procesarea
    pthread_barrier_wait(r_arg->reducer_barrier);

    // inceapa sa extraga numerele din outputul de mapperi, din listele cu puterea respectiva
    for(int i = 0; i < r_arg->mappers; i++) {
        
        // alege la rand un mapper caruia sa ii citeasca outputul
        int** curr_mapper_output = r_arg->outputs[i];

        // incepe sa ii citeasca lista partiala 
        int curr = curr_mapper_output[reducer_id][0];
        int i = 0;
        
        // citeste si insereaza in array pana la elementul -1 ce indica terminarea listei
        while(curr != -1) {
            
            insertArray(&output, curr);
            i++;
            curr = curr_mapper_output[reducer_id][i];
        }
    }

    // sorteaza array-ul
    qsort(output.array, output.used, sizeof(int), compare);

    // numara elementele distincte din fisier
    int k = 0;
    int last_distinct = -1;
    for(int i = 0; i < output.used; i++) {
        if(last_distinct != output.array[i]) {
            k++;
            last_distinct = output.array[i];
        }
    }

    // scrie outputl in fisierul de output respectiv reducerului
    char* file = malloc(20);
    strcpy(file, "out");
    
    char str[10];
    sprintf(str, "%d", reducer_id + 2);
    
    strcat(file, str);
    strcat(file, ".txt");

    FILE* out = fopen(file,"w");
    fprintf(out, "%d", k);
    fclose(out);

    freeArray(&output);

    pthread_exit(NULL);
}