#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "mpi.h"
#include "solver.h"

void solveFirst(int rows, int cols, int iterations, struct timespec ts_sleep, int **matrix, int size, int id)
{
    if (id <= rows - 1)
    {
        int data[9];

        // Iterations
        for (int k = 1; k <= iterations; k++)
        {
            // Colonnes
            for (int col = 0; col < cols; col++)
            {
                nanosleep(&ts_sleep, NULL);
                // Fonction de calcul
                matrix[id][col] = matrix[id][col] + (id + col) * k;
            }
        }

        for (int i = 0; i < 8; i++)
        {
            data[i] = matrix[id][i];
        }
        // Derniere valeur represente la ID du processus
        data[8] = id;

        MPI_Send(data, 9, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

void solveSecond(int rows, int cols, int iterations, struct timespec ts_sleep, int **matrix, int size, int id)
{
    if (id <= rows - 1)
    {
        int data[9];

        // Iteration
        for (int k = 1; k <= iterations; k++)
        {
            // Colonnes
            for (int col = 0; col < cols; col++)
            {
                // Si c'est la premiere colonne de la row
                if (col == 0)
                {
                    nanosleep(&ts_sleep, NULL);
                    // Fonction de calcul
                    matrix[id][0] = matrix[id][0] + (id * k);
                }
                else
                {
                    nanosleep(&ts_sleep, NULL);
                    // Fonction de calcul
                    matrix[id][col] = matrix[id][col] + matrix[id][col - 1] * k;
                }
            }
        }

        for (int i = 0; i < 8; i++)
        {
            data[i] = matrix[id][i];
        }
        // Derniere valeur represente la ID du processus
        data[8] = id;

        MPI_Send(data, 9, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}
