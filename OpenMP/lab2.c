#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

#define ROWS 12
#define COLS 12

int ** allocateMatrix(int rows, int cols) {
    int ** matrix = (int **) malloc(rows * sizeof(int *));

    for(int i = 0; i < rows; i++) {
        matrix[i] = (int *) malloc(cols * sizeof(int));
    }

    return matrix;
}

void deallocateMatrix(int rows, int ** matrix) {
    for(int i = 0; i < rows; i++) {
        free(matrix[i]);
    }

    free(matrix);
}

void fillMatrix(int rows, int cols, int initialValue, int ** matrix) {
     for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            matrix[i][j] = initialValue;
        }
    }
}

void printMatrix(int rows, int cols, int ** matrix) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%12d ", matrix[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

float printRuntime(struct timeval tvs, struct timeval tve) {
    long start = tvs.tv_sec * 1000000 + tvs.tv_usec;
    long end = tve.tv_sec * 1000000 + tve.tv_usec;
    float delta = (end - start) / 1000000.0;
    printf("Runtime: %.6f seconds\n", delta);

    return delta;
}

void printAcceleration(float timeSeq, float timePar) {
    printf("Acceleration: %.6f \n", timeSeq / timePar);
}

void solveFirstSeq(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix) {
    for(int k = 1; k <= iterations; k++) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                nanosleep(&ts_sleep, NULL);
                matrix[row][col] = matrix[row][col] + row + col;
            }
        }
    }
}

void solveFirstPar(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix) {
    for(int k = 1; k <= iterations; k++) {
        #pragma omp parallel for
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                nanosleep(&ts_sleep, NULL);
                matrix[row][col] = matrix[row][col] + row + col;
            }
        }
    }
}

void solveSecondSeq(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix) {
    for(int k = 1; k <= iterations; k++) {
        for (int row = 0; row < rows; row++) {
            for (int col = cols - 1; col >= 0; col--) {
                nanosleep(&ts_sleep, NULL);

                if (col == cols - 1) {
                    matrix[row][col] = matrix[row][col] + row;
                } else {
                    matrix[row][col] = matrix[row][col] + matrix[row][col + 1];
                }
            }
        }
    }
}

void solveSecondPar(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix) {
    for(int k = 1; k <= iterations; k++) {
        #pragma omp parallel for
        for (int row = 0; row < rows; row++) {
            for (int col = cols - 1; col >= 0; col--) {
                nanosleep(&ts_sleep, NULL);

                if (col == cols - 1) {
                    matrix[row][col] = matrix[row][col] + row;
                } else {
                    matrix[row][col] = matrix[row][col] + matrix[row][col + 1];
                }
            }
        }
    }
}

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

    int ** matrix = allocateMatrix(ROWS, COLS);

    if (problem == 1) {
        fillMatrix(ROWS, COLS, initialValue, matrix);
        gettimeofday(&timestamp_s, NULL);
        solveFirstSeq(ROWS, COLS, iterations, ts_sleep, matrix);
        gettimeofday(&timestamp_e, NULL);
        printMatrix(ROWS, COLS, matrix);
        float timeSeq = printRuntime(timestamp_s, timestamp_e);

        fillMatrix(ROWS, COLS, initialValue, matrix);
        gettimeofday(&timestamp_s, NULL);
        solveFirstPar(ROWS, COLS, iterations, ts_sleep, matrix);
        gettimeofday(&timestamp_e, NULL);
        printMatrix(ROWS, COLS, matrix);
        float timePar = printRuntime(timestamp_s, timestamp_e);

        printAcceleration(timeSeq, timePar);
    } else if (problem == 2) {
        fillMatrix(ROWS, COLS, initialValue, matrix);
        gettimeofday(&timestamp_s, NULL);
        solveSecondSeq(ROWS, COLS, iterations, ts_sleep, matrix);
        gettimeofday(&timestamp_e, NULL);
        printMatrix(ROWS, COLS, matrix);
        float timeSeq = printRuntime(timestamp_s, timestamp_e);

        fillMatrix(ROWS, COLS, initialValue, matrix);
        gettimeofday(&timestamp_s, NULL);
        solveSecondPar(ROWS, COLS, iterations, ts_sleep, matrix);
        gettimeofday(&timestamp_e, NULL);
        printMatrix(ROWS, COLS, matrix);
        float timePar = printRuntime(timestamp_s, timestamp_e);

        printAcceleration(timeSeq, timePar);
    }

    deallocateMatrix(ROWS, matrix);

    return EXIT_SUCCESS;
}