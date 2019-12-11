#ifndef MATRIX_HPP
#define MATRIX_HPP

double ** allocateMatrix(int rows, int cols);
void deallocateMatrix(int rows, double ** matrix);
void fillMatrix(int rows, int cols, double ** matrix);
void matrixToArray(double ** matrix, double * array, int rows, int cols);
void arrayToMatrix(double ** matrix, double * array, int rows, int cols);
double * allocateArray(int size, double value);
void copyArray(double * array1, double * array2, int size);

#endif