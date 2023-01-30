#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void messageLogger(int source, int destination) {
    printf("M(%d,%d)\n", source, destination);
}

void printTopology(int rank, int* topology, int size) {
    printf("%d -> ", rank);

    printf("0:");
    int found = 0;
    for(int i = 4; i < size; i++) {
        if(topology[i] == 0) {
            if(found == 0)
                printf("%d", i);
            else
                printf(",%d", i);
            found++;
        }       
    }

    printf(" 1:");
    found = 0;
    for(int i = 4; i < size; i++) {
        if(topology[i] == 1) {
            if(found == 0)
                printf("%d", i);
            else
                printf(",%d", i);
            found++;
        }       
    }

    printf(" 2:");
    found = 0;
    for(int i = 4; i < size; i++) {
        if(topology[i] == 2) {
            if(found == 0)
                printf("%d", i);
            else
                printf(",%d", i);
            found++;
        }       
    }

    printf(" 3:");
    found = 0;
    for(int i = 4; i < size; i++) {
        if(topology[i] == 3) {
            if(found == 0)
                printf("%d", i);
            else
                printf(",%d", i);
            found++;
        }       
    }

    printf("\n");
}

// divides the work as equally as possible between workers
int* divide_tasks(int number_of_workers, int number_of_tasks) {
    int *result = malloc(sizeof(int) * number_of_workers);
    int i = 0;
    int tasks_to_divide = number_of_tasks;

    for(int i = 0; i < number_of_workers; i++)
        result[i] = 0;

    while(tasks_to_divide > 0) {
        result[i]++;
        
        if(i == (number_of_workers - 1))
            i = 0;
        else
            i++;

        tasks_to_divide--;
    }

    return result;
}

// sees how much work has been distributed to a cluster
int work_for_cluster(int* topology, int* work_per_task, int numtasks, int leader) {
    int result = 0;

    for(int i = 4; i < numtasks; i++) {
        if(topology[i] == leader)
            result = result + work_per_task[i - 4];
    }

    return result;
}

void printRezultat(int *result_from_1, int work_for_cluster_1, int *result_from_2, int work_for_cluster_2, 
                    int *result_from_3, int work_for_cluster_3, int *result_from_0, int work_for_cluster_0) {
    printf("Rezultat: ");
    
    for(int i = 0; i < work_for_cluster_1; i++)
        printf("%d ", result_from_1[i]);

    for(int i = 0; i < work_for_cluster_2; i++)
        printf("%d ", result_from_2[i]);

    for(int i = 0; i < work_for_cluster_3; i++)
        printf("%d ", result_from_3[i]);

    for(int i = 0; i < work_for_cluster_0; i++)
        printf("%d ", result_from_0[i]);

    printf("\n");
}


int main (int argc, char *argv[])
{
    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *topology = malloc(sizeof(int) * numtasks);
    MPI_Status status;

    if(rank == 0) {

        // read from file
        FILE* cluster_in = fopen("cluster0.txt", "r");

        int number_of_kids;
        fscanf(cluster_in, "%d\n", &number_of_kids);
        int *kids = malloc(sizeof(int) * number_of_kids);

        topology[0] = 0;
        for(int i = 0; i < number_of_kids; i++) {
            fscanf(cluster_in, "%d\n", &kids[i]);

            topology[kids[i]] = 0;
        }

        // learn topology from other cluster leaders

        // learn from cluster1
        int number_of_kids_cluster_1;
        MPI_Recv(&number_of_kids_cluster_1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        int *cluster1_kids = malloc(sizeof(int) * number_of_kids_cluster_1);
        MPI_Recv(cluster1_kids, number_of_kids_cluster_1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

        for(int i = 0; i < number_of_kids_cluster_1; i++) {
            topology[cluster1_kids[i]] = 1;
        }

        //learn from cluster2
        int number_of_kids_cluster_2;
        MPI_Recv(&number_of_kids_cluster_2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        int *cluster2_kids = malloc(sizeof(int) * number_of_kids_cluster_2);
        MPI_Recv(cluster2_kids, number_of_kids_cluster_2, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

        for(int i = 0; i < number_of_kids_cluster_2; i++) {
            topology[cluster2_kids[i]] = 2;
        }

        //learn from cluster3
        int number_of_kids_cluster_3;
        MPI_Recv(&number_of_kids_cluster_3, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);
        int *cluster3_kids = malloc(sizeof(int) * number_of_kids_cluster_3);
        MPI_Recv(cluster3_kids, number_of_kids_cluster_3, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);

        for(int i = 0; i < number_of_kids_cluster_3; i++) {
            topology[cluster3_kids[i]] = 3;
        }

        printTopology(rank, topology, numtasks);

        // Send topology to cluster leaders
        MPI_Send(topology, numtasks, MPI_INT, 1, 0, MPI_COMM_WORLD);
        messageLogger(rank, 1);

        MPI_Send(topology, numtasks, MPI_INT, 3, 0, MPI_COMM_WORLD);
        messageLogger(rank, 3);

        // Send topology to kids
        for(int i = 0; i < number_of_kids; i++) {
            MPI_Send(topology, numtasks, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);
        }

        // Generate vector
        int vector_size = atoi(argv[1]);
        int *initial_vector = malloc(sizeof(int) * vector_size);

        for(int i = 0; i < vector_size; i++) {
            initial_vector[i] = vector_size - i - 1;
        }

        // Divide tasks
        int number_of_workers = numtasks - 4;
        int *tasks_per_worker = divide_tasks(number_of_workers, vector_size);        
        
        // Send cluster1 his work
        int work_for_cluster_1 = work_for_cluster(topology, tasks_per_worker, numtasks, 1);
        MPI_Send(&work_for_cluster_1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        messageLogger(0, 1);
        MPI_Send(initial_vector, work_for_cluster_1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        messageLogger(0, 1);

        // Receive back work from cluster1
        int *result_from_1 = malloc(work_for_cluster_1 * sizeof(int));
        MPI_Recv(result_from_1, work_for_cluster_1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

        // Send cluster2 his work (through 3)
        int work_for_cluster_2 = work_for_cluster(topology, tasks_per_worker, numtasks, 2);
        MPI_Send(&work_for_cluster_2, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        messageLogger(0, 3);
        MPI_Send(initial_vector + work_for_cluster_1, work_for_cluster_2, MPI_INT, 3, 0, MPI_COMM_WORLD);
        messageLogger(0, 3);

        // Receive work from cluster2 (thorugh 3)
        int *result_from_2 = malloc(work_for_cluster_2 * sizeof(int));
        MPI_Recv(result_from_2, work_for_cluster_2, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);

        // Send cluster3 his work
        int work_for_cluster_3 = work_for_cluster(topology, tasks_per_worker, numtasks, 3);
        MPI_Send(&work_for_cluster_3, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        messageLogger(0, 3);
        MPI_Send(initial_vector + work_for_cluster_1 + work_for_cluster_2, work_for_cluster_3, MPI_INT, 3, 0, MPI_COMM_WORLD);
        messageLogger(0, 3);

        // Receive back work from cluster3
        int *result_from_3 = malloc(work_for_cluster_3 * sizeof(int));
        MPI_Recv(result_from_3, work_for_cluster_3, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);

        // Send own workers their work
        int work_for_cluster_0 = work_for_cluster(topology, tasks_per_worker, numtasks, 0);
        int *work_vector = initial_vector + work_for_cluster_1 + work_for_cluster_2 + work_for_cluster_3;
        
        // Divide work for workers;
        int *tasks_per_worker_0 = divide_tasks(number_of_kids, work_for_cluster_0);

        for(int i = 0; i < number_of_kids; i++) {

            MPI_Send(&tasks_per_worker_0[i], 1, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            MPI_Send(work_vector, tasks_per_worker_0[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            work_vector = work_vector + tasks_per_worker_0[i];
        }
        
        // Receive work from workers
        int *result_from_0 = malloc(work_for_cluster_0 * sizeof(int));
        int k = 0;

        for(int i = 0; i < number_of_kids; i++) {
            MPI_Recv(result_from_0 + k, tasks_per_worker_0[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD, &status);
            k = k + tasks_per_worker_0[i];
        }

        //for(int i = 0; i < work_for_cluster_0; i++)
            //printf("work from 0: [%d] %d\n", i, result_from_0[i]);
        
        printRezultat(result_from_1, work_for_cluster_1, result_from_2, work_for_cluster_2, result_from_3, work_for_cluster_3, result_from_0, work_for_cluster_0);

    } else if(rank == 1) {
        
        // read from file
        FILE* cluster_in = fopen("cluster1.txt", "r");
        
        int number_of_kids;
        fscanf(cluster_in, "%d\n", &number_of_kids);
        int *kids = malloc(sizeof(int) * number_of_kids);

        for(int i = 0; i < number_of_kids; i++) {
            fscanf(cluster_in, "%d\n", &kids[i]);
        }

        // Send rank 0 the kids
        MPI_Send(&number_of_kids, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(rank, 0);

        MPI_Send(kids, number_of_kids, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(rank, 0);

        // Send rank 0 cluster2 kids
        int number_of_kids_cluster_2;
        MPI_Recv(&number_of_kids_cluster_2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
        int *cluster2_kids = malloc(sizeof(int) * number_of_kids_cluster_2);
        MPI_Recv(cluster2_kids, number_of_kids_cluster_2, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);

        MPI_Send(&number_of_kids_cluster_2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(rank, 0);

        MPI_Send(cluster2_kids, number_of_kids_cluster_2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(rank, 0);

        // Receive topology
        MPI_Recv(topology, numtasks, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printTopology(rank, topology, numtasks);

        // Send topology to cluster2
        MPI_Send(topology, numtasks, MPI_INT, 2, 0, MPI_COMM_WORLD);
        messageLogger(rank, 2);

        // Send topology to kids
        for(int i = 0; i < number_of_kids; i++) {
            MPI_Send(topology, numtasks, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);
        }

        // Receive work from rank 0
        int number_of_tasks;
        MPI_Recv(&number_of_tasks, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int *work = malloc(number_of_tasks * sizeof(int));
        MPI_Recv(work, number_of_tasks, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        // Divide work for workers;
        int *tasks_per_worker = divide_tasks(number_of_kids, number_of_tasks);

        
        for(int i = 0; i < number_of_kids; i++) {

            MPI_Send(&tasks_per_worker[i], 1, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            MPI_Send(work, tasks_per_worker[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            work = work + tasks_per_worker[i];
        }
        
        // Receive work from workers
        
        work = malloc(number_of_tasks * sizeof(int));
        int k = 0;

        for(int i = 0; i < number_of_kids; i++) {
            MPI_Recv(work + k, tasks_per_worker[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD, &status);
            k = k + tasks_per_worker[i];
        }

        
        // Send back work to rank 0
        MPI_Send(work, number_of_tasks, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(1, 0);

    } else if(rank == 2) {

        // read from file
        FILE* cluster_in = fopen("cluster2.txt", "r");

        int number_of_kids;
        fscanf(cluster_in, "%d\n", &number_of_kids);
        int *kids = malloc(sizeof(int) * number_of_kids);

        for(int i = 0; i < number_of_kids; i++) {
            fscanf(cluster_in, "%d\n", &kids[i]);
        }

        // Send rank 1 the kids
        MPI_Send(&number_of_kids, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        messageLogger(rank, 1);

        MPI_Send(kids, number_of_kids, MPI_INT, 1, 0, MPI_COMM_WORLD);
        messageLogger(rank, 1);

        // Receive topology
        MPI_Recv(topology, numtasks, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        printTopology(rank, topology, numtasks);

        // Send topology to kids
        for(int i = 0; i < number_of_kids; i++) {
            MPI_Send(topology, numtasks, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);
        }

        // Receive work from rank 3
        int number_of_tasks;
        MPI_Recv(&number_of_tasks, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);
        int *work = malloc(number_of_tasks * sizeof(int));
        MPI_Recv(work, number_of_tasks, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);
        
        // Divide work for workers;
        int *tasks_per_worker = divide_tasks(number_of_kids, number_of_tasks);

        for(int i = 0; i < number_of_kids; i++) {

            MPI_Send(&tasks_per_worker[i], 1, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            MPI_Send(work, tasks_per_worker[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            work = work + tasks_per_worker[i];
        }
        
        // Receive work from workers
        work = malloc(number_of_tasks * sizeof(int));
        int k = 0;

        for(int i = 0; i < number_of_kids; i++) {
            MPI_Recv(work + k, tasks_per_worker[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD, &status);
            k = k + tasks_per_worker[i];
        }

        // Send back work to rank 0
        MPI_Send(work, number_of_tasks, MPI_INT, 3, 0, MPI_COMM_WORLD);
        messageLogger(2, 3);

    } else if(rank == 3) {

        // read from file
        FILE* cluster_in = fopen("cluster3.txt", "r");

        int number_of_kids;
        fscanf(cluster_in, "%d\n", &number_of_kids);
        int *kids = malloc(sizeof(int) * number_of_kids);

        for(int i = 0; i < number_of_kids; i++) {
            fscanf(cluster_in, "%d\n", &kids[i]);
        }

        // Send rank 0 the kids
        MPI_Send(&number_of_kids, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(rank, 0);

        MPI_Send(kids, number_of_kids, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(rank, 0);

        // Receive topology
        MPI_Recv(topology, numtasks, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printTopology(rank, topology, numtasks);

        // Send topology to kids
        for(int i = 0; i < number_of_kids; i++) {
            MPI_Send(topology, numtasks, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);
        }

        // Receive work from rank 0 for cluster2
        int number_of_tasks_for_2;
        MPI_Recv(&number_of_tasks_for_2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int *work_for_2 = malloc(number_of_tasks_for_2 * sizeof(int));
        MPI_Recv(work_for_2, number_of_tasks_for_2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        // Send cluster2 his work
        MPI_Send(&number_of_tasks_for_2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        messageLogger(3, 2);
        MPI_Send(work_for_2, number_of_tasks_for_2, MPI_INT, 2, 0, MPI_COMM_WORLD);
        messageLogger(3, 2);

        // Receive work from rank 2 and send it to 0
        MPI_Recv(work_for_2, number_of_tasks_for_2, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
        MPI_Send(work_for_2, number_of_tasks_for_2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(3, 0);

        // Receive work from rank 0
        int number_of_tasks;
        MPI_Recv(&number_of_tasks, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int *work = malloc(number_of_tasks * sizeof(int));
        MPI_Recv(work, number_of_tasks, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        // Divide work for workers;
        int *tasks_per_worker = divide_tasks(number_of_kids, number_of_tasks);

        for(int i = 0; i < number_of_kids; i++) {

            MPI_Send(&tasks_per_worker[i], 1, MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            MPI_Send(work, tasks_per_worker[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD);
            messageLogger(rank, kids[i]);

            work = work + tasks_per_worker[i];
        }
        
        // Receive work from workers
        work = malloc(number_of_tasks * sizeof(int));
        int k = 0;

        for(int i = 0; i < number_of_kids; i++) {
            MPI_Recv(work + k, tasks_per_worker[i], MPI_INT, kids[i], 0, MPI_COMM_WORLD, &status);
            k = k + tasks_per_worker[i];
        }

        // Send back work to rank 0
        MPI_Send(work, number_of_tasks, MPI_INT, 0, 0, MPI_COMM_WORLD);
        messageLogger(3, 0);

    } else {

        // Receive topology
        MPI_Recv(topology, numtasks, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        printTopology(rank, topology, numtasks);

        // Receive work
        
        int number_of_tasks;
        MPI_Recv(&number_of_tasks, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        int* work = malloc(sizeof(int) * number_of_tasks);
        MPI_Recv(work, number_of_tasks, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        // Work
        for(int i = 0; i < number_of_tasks; i++) {
            work[i] = work[i] * 5;
        }
        
        // Send back work
        MPI_Send(work, number_of_tasks, MPI_INT, topology[rank], 0, MPI_COMM_WORLD);
        messageLogger(rank, topology[rank]);

    }

    MPI_Finalize();
	return 0;
}