#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "mpi.h"
#include "solver.h"

void solveFirst(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix, int size, int id) {    
    int data[3];
    int row = id / rows;
    int col = id % cols;

    for(int k = 1; k <= iterations; k++) {
        nanosleep(&ts_sleep, NULL);
        matrix[row][col] = matrix[row][col] + (row + col) * k;
    }

    data[0] = row;
    data[1] = col;
    data[2] = matrix[row][col];

    MPI_Send(data, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

void solveSecond(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix, int size, int id) {
    for(int k = 1; k <= iterations; k++) {
        for(int i = 0; i < rows; i++) {
            nanosleep(&ts_sleep, NULL);
            matrix[i][0] = matrix[i][0] + (i * k);
        }

        for(int j = 1; j < cols; j++) {
            for(int i = 0; i < rows; i++) {
                nanosleep(&ts_sleep, NULL);
                matrix[i][j] = matrix[i][j] + matrix[i][j - 1] * k;
            }
        }
    }
}
