#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "mapper.h"


// Functia ce verifica daca number e putere power de ceva (cautare binara)
int verify_pow(int number, int power) {

    int start = 1;
    int end = number;
    int curr;
    int last = -1;
    int ok = 0;

    if(number == 1)
        return 1;

    while(start < end) {
        curr = (int)((end + start) / 2);

        if(curr == last)
            break;

        if(pow(curr, power) == number) {
            ok = 1;
            break;
        }

        if(pow(curr, power) > number) {
            end = curr;
            last = curr;
        } else {
            start = curr;
            last = curr;
        }
    }

    return ok;
}


void *mapper_function(void* arg) {
    
    // initializare structura mapper_arg
    struct Mapper_arg *m_arg = (struct Mapper_arg*)arg;
    
    // variabile pentru citire din fisier
    char * line;
	size_t len = 0;
    int finished;

    // initializare marime output (lista de liste de int)
    int used[m_arg->reducer];
    for (int i = 0; i < m_arg->reducer; i++) {
        used[i] = 0;
    }  

    //int output_size = 1;

    // primul mapper citeste numarul de fisiere (nu este folosit deloc in implementarea mea)
    if(m_arg->thread_id == 0) {
        getline(&line, &len, m_arg->file);
    }
        

    while(1) {

        // Mutex pentru citirea unul nu nume de fisier
        pthread_mutex_lock(m_arg->file_reading_mutex);
        finished = getline(&line, &len, m_arg->file);
        pthread_mutex_unlock(m_arg->file_reading_mutex);

        // Daca s-a terminat de citit din fisier intrerupe
        if(finished < 0)
            break;

        if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
	    if (line[strlen(line) - 1] == '\r')
	        line[strlen(line) - 1] = '\0';

        // Initializeaza si citeste din fisierul tocmai luat din zona critica
        FILE* input;
        input = fopen(line, "r");

        char* number_line;
        size_t number_len = 0;

        getline(&number_line, &number_len, input);
        if (number_line[strlen(line) - 1] == '\n')
		    number_line[strlen(line) - 1] = '\0';
        if (number_line[strlen(line) - 1] == '\r')
		    number_line[strlen(line) - 1] = '\0';
        
        // Citeste numerele pe rand
        while(getline(&number_line, &number_len, input) != -1) {
            
            if (number_line[strlen(line) - 1] == '\n')
			    number_line[strlen(line) - 1] = '\0';
            if (number_line[strlen(line) - 1] == '\r')
			    number_line[strlen(line) - 1] = '\0';

            int number = atoi(number_line);
            
            int i = 1;

            // verifica fiecare daca e putere x de ceva pentru fiecare x in functie de cati reduceri sunt
            while(i - 1 < m_arg->reducer) {
                if(verify_pow(number, i + 1) == 1) {

                    // Daca este introduce in lista partiala necesara
                    m_arg->output[i-1][used[i-1]] = number;
                    used[i-1]++;
                }
                i++;
            }
        }

        fclose(input);    
    }

    // numarul -1 indica terminarea listei pentru reduceri
    for(int i = 0; i < m_arg->reducer; i++) {
        m_arg->output[i][used[i]] = -1;
    }

    // bariera
    pthread_barrier_wait(m_arg->reducer_barrier);
    pthread_exit(NULL);
}