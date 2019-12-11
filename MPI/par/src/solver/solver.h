#ifndef SOLVER_H
#define SOLVER_H

void solveFirst(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix, int size, int id);
void solveSecond(int rows, int cols, int iterations, struct timespec ts_sleep, int ** matrix, int size, int id);

#endif
