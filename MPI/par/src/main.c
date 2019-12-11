#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include "mpi.h"
#include "matrix/matrix.h"
#include "output/output.h"
#include "solver/solver.h"

#define ROWS 8
#define COLS 8

void (* solve)(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix, int size, int id) = solveFirst;

int main(int argc, char* argv[]) {
    if(4 != argc) {
        return EXIT_FAILURE;
    }

    struct timeval timestamp_s;
    struct timeval timestamp_e;

    struct timespec ts_sleep;
    ts_sleep.tv_sec = 0;
    ts_sleep.tv_nsec = 1000000L;

    int problem = atoi(argv[1]);
    int initialValue = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    int err;

    err = MPI_Init(&argc, &argv);

    if (err != MPI_SUCCESS) 
    {
        printf("Erreur init MPI");
        exit(1);
    }

    int size, id;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    void * solvers[2];
    solvers[0] = solveFirst;
    solvers[1] = solveSecond;

    solve = solvers[problem - 1];

    int ** matrix = allocateMatrix(ROWS, COLS);
    fillMatrix(ROWS, COLS, initialValue, matrix);

    gettimeofday(&timestamp_s, NULL);
    solve(ROWS, COLS, iterations, ts_sleep, matrix, size, id);
    gettimeofday(&timestamp_e, NULL);

    MPI_Barrier(MPI_COMM_WORLD);

    if (id == 0) {
        int receivedCount = 1;
        while (receivedCount <= size) {
            int receivedData[3];
            MPI_Status status;
            
            MPI_Recv(&receivedData, 3, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            matrix[receivedData[0]][receivedData[1]] = receivedData[2];

            receivedCount++;
        }

        printMatrix(ROWS, COLS, matrix);
        printRuntime(timestamp_s, timestamp_e);
    }
    
    deallocateMatrix(ROWS, matrix);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
