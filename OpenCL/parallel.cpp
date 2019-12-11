#define errCheck(code) { errorCheck(code, __FILE__, __LINE__); }

#include <iostream>
#include <fstream>
#include <CL/opencl.h>

#include "windows.h"
#include "parallel.hpp"
#include "matrix.hpp"

char * readFile(const char * fileName);
void addWithOpenCl(double ** matrix, int rows, int cols, int iters, double td, double h, const char * kernelSource);

using std::cout;
using std::flush;
using std::endl;

inline void errorCheck(cl_int code, const char* file, int line) {
	if (CL_SUCCESS != code) {
		std::cout << "[" << file << ", line " << line << "]" << std::flush;
		std::cout << " OpenCL error code <" << code << "> received." << std::endl << std::flush;
		exit(EXIT_FAILURE);
	}
}

void solvePar(int rows, int cols, int iterations, double td, double h, double ** matrix, const char * kernelFileName) {
	char * kernelSource = readFile(kernelFileName);

	addWithOpenCl(matrix, rows, cols, iterations, td, h, kernelSource);
}

char * readFile(const char * fileName) {
	int length;

	std::ifstream file(fileName, std::ifstream::in | std::ios::binary);
	file.seekg(0, std::ios::end);
	length = file.tellg();
	file.seekg(0, std::ios::beg);

	char * buffer = (char *) malloc(length + 1);
	file.read(buffer, length);
	file.close();

	buffer[length] = '\0';

	return buffer;
}

void addWithOpenCl(double ** matrix, int rows, int cols, int iters, double td, double h, const char * kernelSource) {
	double * inArray = allocateArray(rows * cols, 0);
	double * outArray = allocateArray(rows * cols, 0);
	int bytes = rows * cols * sizeof(double);
	int isReversed = 0;

	matrixToArray(matrix, inArray, rows, cols);

	cl_int err = CL_SUCCESS;

	// Get execution platform.
	cl_platform_id platform;
	errCheck(clGetPlatformIDs(1, &platform, NULL));

	// Get available gpus on platform.
	cl_device_id device_id;
	errCheck(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL));

	// Create an execution context.
	cl_context context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	errCheck(err);

	// Create the command queue.
	cl_command_queue queue = clCreateCommandQueue(context, device_id, NULL, &err);
	errCheck(err);

	// Compile the source program.
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &err);
	errCheck(err);

	errCheck(clBuildProgram(program, 0, NULL, NULL, NULL, NULL));

	// Setup an execution kernel from the source program.
	cl_kernel kernel = clCreateKernel(program, "addKernel", &err);
	errCheck(err);

	// Create device buffers.
	cl_mem inArrayBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, &err);
	errCheck(err);
	cl_mem outArrayBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, &err);
	errCheck(err);

	// Write host data to the device.
	errCheck(clEnqueueWriteBuffer(queue, inArrayBuffer, CL_TRUE, 0, bytes, inArray, 0, NULL, NULL));
	errCheck(clEnqueueWriteBuffer(queue, outArrayBuffer, CL_TRUE, 0, bytes, outArray, 0, NULL, NULL));

	// Setup function arguments.
	errCheck(clSetKernelArg(kernel, 0, sizeof(cl_mem), &inArrayBuffer));
	errCheck(clSetKernelArg(kernel, 1, sizeof(cl_mem), &outArrayBuffer));
	errCheck(clSetKernelArg(kernel, 2, sizeof(int), &rows));
	errCheck(clSetKernelArg(kernel, 3, sizeof(int), &cols));
	errCheck(clSetKernelArg(kernel, 4, sizeof(double), &td));
	errCheck(clSetKernelArg(kernel, 5, sizeof(double), &h));

	// Execute the kernel.
	size_t globalSize = (rows - 2) * (cols - 2);

	for (int k = 0; k < iters; k++) {

		isReversed = k % 2 == 0 ? 0 : 1;

		errCheck(clSetKernelArg(kernel, 6, sizeof(int), &isReversed));
		errCheck(clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL));
	}

	errCheck(clFinish(queue));
	errCheck(clEnqueueReadBuffer(queue, inArrayBuffer, CL_TRUE, 0, bytes, inArray, 0, NULL, NULL));
	errCheck(clEnqueueReadBuffer(queue, outArrayBuffer, CL_TRUE, 0, bytes, outArray, 0, NULL, NULL));

	if (isReversed == 1) {
		arrayToMatrix(matrix, inArray, rows, cols);
	} else {
		arrayToMatrix(matrix, outArray, rows, cols);
	}

	// Clear memory.
	errCheck(clReleaseMemObject(inArrayBuffer));
	errCheck(clReleaseMemObject(outArrayBuffer));
	errCheck(clReleaseKernel(kernel));
	errCheck(clReleaseProgram(program));
	errCheck(clReleaseCommandQueue(queue));
	errCheck(clReleaseContext(context));
}