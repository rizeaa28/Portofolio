#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "mapper.h"

struct Reducer_arg {
    int thread_id;                          // thread id
    int mappers;                            // numarul de mapperi
    int*** outputs;                         // outputul mapperilor
    pthread_barrier_t* reducer_barrier;     // bariera pentru sincronizare mapper-reducer
};

void *reducer_function(void*);