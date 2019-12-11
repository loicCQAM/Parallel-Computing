#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include "mpi.h"
#include "matrix/matrix.h"
#include "output/output.h"
#include "solver/solver.h"

#define WAIT_TIME 5 

int main(int argc, char *argv[])
{
    int n, m, np, td, h;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    np = atoi(argv[3]);
    td = atof(argv[4]);
    h = atof(argv[5]);

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

    int **matrix = allocateMatrix(n, m);
    fillMatrix(n, m, 0, matrix);

    struct timeval timestamp_s;
    struct timeval timestamp_e;

    gettimeofday(&timestamp_s, NULL);
    gettimeofday(&timestamp_e, NULL);

    MPI_Barrier(MPI_COMM_WORLD);

    if (id == 0)
    {
        printMatrix(n, m, matrix);
        printRuntime(timestamp_s, timestamp_e);
    } else {
	printf("process ID: %d\n", id);
    }

    deallocateMatrix(n, matrix);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
