#include "matrix.hpp"

double ** allocateMatrix(int rows, int cols) {
    double ** matrix = new double*[rows];

    for(int i = 0; i < rows; i++) {
        matrix[i] = new double[cols];
    }

    return matrix;
}

void deallocateMatrix(int rows, double ** matrix) {
    for(int i = 0; i < rows; i++) {
        delete(matrix[i]);
    }

    delete(matrix);
}

void fillMatrix(int rows, int cols, double ** matrix) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			matrix[row][col] = row * (rows - row - 1) * (2.0 * row / rows) * col * (cols - col - 1) * (1.0 * col / cols);
		}
	}
}

void matrixToArray(double ** matrix, double * array, int rows, int cols) {
	int i = 0;

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			array[i] = matrix[row][col];
			i++;
		}
	}
}

void arrayToMatrix(double ** matrix, double * array, int rows, int cols) {
	int i = 0;

	for (int i = 0; i < rows * cols; i++) {
		int row = i / cols;
		int col = i % cols;

		matrix[row][col] = array[i];
	}
}

double * allocateArray(int size, double value) {
	double * array = new double[size];

	for (int i = 0; i < size; i++) {
		array[i] = value;
	}

	return array;
}

void copyArray(double * array1, double * array2, int size) {
	for (int i = 0; i < size; i++) {
		array2[i] = array1[i];
	}
}