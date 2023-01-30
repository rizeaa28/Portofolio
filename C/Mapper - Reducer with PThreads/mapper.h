#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "array.h"
#include <string.h>

struct Mapper_arg {
    int thread_id;                          // thread id
    FILE* file;                             // fisierul cu toate fisierele
    pthread_mutex_t* file_reading_mutex;    // mutexul pentru citirea in fisier
    pthread_barrier_t* reducer_barrier;     // bariera pentru sincronizare mapper-reducer
    int reducer;                            // numarul de reducer pentru a sti puterea maxima
    int** output;                           // lista de liste ca output
};

void *mapper_function(void*);
