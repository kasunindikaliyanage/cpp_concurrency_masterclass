#pragma once
#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>

#include "utils.h"
#include "common_objs.h"

class Matrix {

	int* data;
	int rows; // row count
 	int columns; // column count

public:

	Matrix(int _n, int _m) : rows(_n), columns(_m)
	{
		data = new int[ rows * columns ];
		//set the array to 0
		std::fill(data, data + rows*columns, 0);
	}

	// i -> 0 to n-1
	// j -> 0 to m-1
	void set_value(int i, int j, int value)
	{
		data[i * columns + j] = value;
	}

	void set_all( int value)
	{
		std::fill(data, data + rows * columns, value);
	}

	static void multiply(Matrix* x, Matrix* y, Matrix* results)
	{
		//check the matrix sizes are correct to multiply
		if (!(x->columns == y->rows) || !((x->rows == results->rows) && (y->columns == results->columns)))
		{
			std::cout << " ERROR : Invalid matrix sizes for multiplication \n";
			return;
		}

		// r = result_size
		int r = results->rows * results->columns;

		for (size_t i = 0; i < r; i++)
		{
			for (size_t j = 0; j < x->columns; j++)
			{
				results->data[i] += x->data[ (i / results->columns) * x->columns + j ] 
					* y->data[ i % results->rows + j*y->columns ];
			}	
		}
	}

	static void parallel_multiply(Matrix* x, Matrix* y, Matrix* results)
	{
		struct process_data_chunk
		{
			void operator()(Matrix* results, Matrix* x, Matrix* y, int start_index, int end_index)
			{
				for (size_t i = start_index; i < end_index; i++)
				{
					for (size_t j = 0; j < x->columns; j++)
					{
						results->data[i] += x->data[(i / results->columns) * x->columns + j]
							* y->data[i % results->rows + j * y->columns];
					}
				}
			}

		};
		
		//check the matrix sizes are correct to multiply
		if (!((x->rows == results->rows) && (y->columns == results->columns)) || !(x->columns == y->rows))
		{
			std::cout << " ERROR : Invalid matrix sizes for multiplication \n";
		}

		// r = result_size
		int length = results->rows * results->columns;

		if (!length)
			return;

		int min_per_thread = 10000;
		int max_threads = (length + min_per_thread - 1) / min_per_thread;
		int hardware_threads = std::thread::hardware_concurrency();
		int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
		int block_size = length / num_threads;

		std::vector<std::thread> threads(num_threads - 1);
		int block_start = 0;
		int block_end = 0;
		{
			join_threads joiner(threads);

			for (unsigned long i = 0; i < (num_threads - 1); i++)
			{
				block_start = i * block_size;
				block_end = block_start + block_size;
				threads[i] = std::thread(process_data_chunk(), results, x, y, block_start, block_end);
			}

			// perform the find operation for final block in this thread.
			process_data_chunk()(results, x, y, block_end, length);
		}
	}

	static void transpose(Matrix* x,  Matrix* results)
	{
		//check the matrix sizes are correct to multiply
		if ( !((x->columns == results->rows) && (x->rows == results->columns)) )
		{
			std::cout << " ERROR : Invalid matrix sizes for transpose \n";
			return;
		}

		// r = result_size
		int r = results->rows * results->columns;

		int result_column = 0;
		int result_row = 0;

		int input_column = 0;
		int input_row = 0;

		for (size_t i = 0; i < r; i++)
		{
			//get the current row and column count
			result_row = i / results->columns;
			result_column = i % results->columns;

			//flipped the columns and row for input
			input_row = result_column;
			input_column = result_row;

			//store the corresponding element from input to the results
			results->data[i] = x->data[input_row * x->columns + input_column];
		}
	}

	static void parallel_transpose(Matrix* x,  Matrix* results)
	{
		struct process_data_chunk
		{
			void operator()(Matrix* results, Matrix* x, int start_index, int end_index)
			{
				int result_column = 0;
				int result_row = 0;

				int input_column = 0;
				int input_row = 0;

				for (size_t i = start_index; i < end_index; i++)
				{
					result_row = i / results->columns;
					result_column = i % results->columns;

					input_row = result_column;
					input_column = result_row;

					results->data[i] = x->data[input_row * x->columns + input_column];
				}
			}

		};

		//check the matrix sizes are correct to multiply
		if (!((x->columns == results->rows) && (x->rows == results->columns)))
		{
			std::cout << " ERROR : Invalid matrix sizes for transpose \n";
			return;
		}

		// r = result_size
		int length = results->rows * results->columns;

		if (!length)
			return;

		int min_per_thread = 10000;
		int max_threads = (length + min_per_thread - 1) / min_per_thread;
		int hardware_threads = std::thread::hardware_concurrency();
		int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
		int block_size = length / num_threads;

		std::vector<std::thread> threads(num_threads - 1);
		int block_start = 0;
		int block_end = 0;
		{
			join_threads joiner(threads);

			for (unsigned long i = 0; i < (num_threads - 1); i++)
			{
				block_start = i * block_size;
				block_end = block_start + block_size;
				threads[i] = std::thread(process_data_chunk(), results, x, block_start, block_end);
			}

			// perform the find operation for final block in this thread.
			process_data_chunk()(results, x, block_end, length);
		}
	}

	void print()
	{
		if ( rows < 50 && columns < 50 )
		{
			for (size_t i = 0; i < rows; i++)
			{
				for (size_t j = 0; j < columns; j++)
				{
					std::cout << data[j + i * columns] << " ";
				}

				std::cout << "\n";
			}
			std::cout << std::endl;
		}
	}

	~Matrix()
	{
		delete data;
	}
};