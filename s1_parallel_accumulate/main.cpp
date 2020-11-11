#include <iostream>
#include <thread>
#include <stdexcept>

#include "parallel_accumulate.h"

int  main()
{
	int result = 0;
	std::vector<int> vec(10000);
	for (int i = 0; i < 10000; i++)
		vec[i] = 2;

	parallel_accumulate(vec.begin(), vec.end(), result);

	std::cout << "final result = " << result << std::endl;
}