__kernel void addKernel(__global double * inArray, __global double * outArray, int rows, int cols, double td, double h, int isReversed) {
    int id = get_global_id(0);
	int row = id / (cols - 2) + 1;
	int col = id % (cols - 2) + 1;
	int position = (row * cols) + col;
	int prevRowPosition = position - cols;
	int nextRowPosition = position + cols;
	int prevColPosition = position - 1;
	int nextColPosition = position + 1;

	//printf("pos: %d:%d\n", row, col);
	//printf("position: %d\n", position);
	//printf("reversed: %d \n", isReversed);

	if (isReversed == 1) {
		inArray[position] = (1 - ((4 * td) / (h * h))) * outArray[position] + (td / h * h) * (outArray[prevRowPosition] + outArray[nextRowPosition] + outArray[prevColPosition] + outArray[nextColPosition]);
	} else {
		outArray[position] = (1 - ((4 * td) / (h * h))) * inArray[position] + (td / h * h) * (inArray[prevRowPosition] + inArray[nextRowPosition] + inArray[prevColPosition] + inArray[nextColPosition]);
	}
}