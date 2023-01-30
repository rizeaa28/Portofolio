#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "reducer.h"

// functie ce proceseaza argumentele date la rulare
void get_args(int argc, char **argv, int *mapper, int *reducer, char* filename)
{
	if(argc < 4) {
		printf("Numar insuficient de parametri: ./tema1 nr_mapper nr_reducer filename\n");
		exit(1);
	}

	*mapper = atoi(argv[1]);
	*reducer = atoi(argv[2]);
    strcpy(filename, argv[3]);
}



int main(int argc, char *argv[]) {
    
    // numarul de mapperi/reduceri
    int mapper, reducer;
    
    // fisierul de intrare
    char* filename = malloc(100);
    FILE *fi;
    
    // initializarea elementelor de sincronizare
    pthread_mutex_t* file_reading_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_barrier_t reducer_barrier;

    get_args(argc, argv, &mapper, &reducer, filename);

    pthread_mutex_init(file_reading_mutex, NULL);
    pthread_barrier_init(&reducer_barrier, NULL, mapper+reducer);

    // initializarea structurii unde se vor tine puterile
    int ***mapper_outputs = (int***)malloc(mapper * sizeof(int**));

    for(int i = 0; i < mapper; i++) {
        mapper_outputs[i] = (int**)malloc(reducer * sizeof(int*));

        for (int j = 0; j < reducer; j++)
            mapper_outputs[i][j] = malloc(5000 * sizeof(int));
    }

    fi = fopen(filename, "r"); //fisierul de input

    // start thread-uri
    pthread_t tid[mapper + reducer];
	int thread_id[mapper + reducer];

    for (int i = 0; i < mapper + reducer; i++) {
		thread_id[i] = i;

        if(i < mapper) { // Daca threadul este mapper 
            
            struct Mapper_arg *arg = malloc(sizeof(struct Mapper_arg));
            arg->thread_id = thread_id[i];
            arg->file = fi;
            arg->file_reading_mutex = file_reading_mutex;
            arg->reducer_barrier = &reducer_barrier;
            arg->reducer = reducer;
            arg->output = mapper_outputs[i];

            pthread_create(&tid[i], NULL, mapper_function, arg);
        }  
        else { // Daca threadul este reducer
            
            struct Reducer_arg *arg = malloc(sizeof(struct Reducer_arg));
            arg->thread_id = thread_id[i];
            arg->mappers = mapper;
            arg->outputs = mapper_outputs;
            arg->reducer_barrier = &reducer_barrier;

            pthread_create(&tid[i], NULL, reducer_function, arg);
	    }
    }
          

    // Join la thread-uri
    for (int i = 0; i < mapper + reducer; i++) {
		int r = pthread_join(tid[i], NULL);
        
        if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

    pthread_mutex_destroy(file_reading_mutex);
    pthread_barrier_destroy(&reducer_barrier);
    return 0;
}

