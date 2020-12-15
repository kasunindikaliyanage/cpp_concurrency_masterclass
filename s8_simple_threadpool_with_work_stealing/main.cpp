#include <iostream>
#include <future>
#include <numeric>

#include "simple_threadpool_with_work_stealing.h"

int main()
{
	const int size = 800;
	std::list<int> my_array;

	srand(0);

	for (size_t i = 0; i < size; i++)
	{
		my_array.push_back(rand());
	}

	my_array = parallel_quick_sort(my_array);

	for (size_t i = 0; i < size; i++)
	{
		std::cout << my_array.front() << std::endl;
		my_array.pop_front();
	}
}