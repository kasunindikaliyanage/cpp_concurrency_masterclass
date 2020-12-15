#include <iostream>
#include <future>
#include <numeric>

#include "simple_thread_pool_with_waiting.h"

int main()
{
	std::cout << "Simple thread pool with waiting \n";
	const int size = 1000;
	int* my_array = new int[size];

	srand(0);

	for (size_t i = 0; i < size; i++)
	{
		//my_array[i] = rand() % 10;
		my_array[i] = 1;
	}

	long result = parallel_accumulate<int*, int>(my_array, my_array + size, 0);
	std::cout << "final sum is  - " << result << std::endl;
	return 0;
}