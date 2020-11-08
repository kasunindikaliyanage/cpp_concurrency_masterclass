#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <execution>
#include <string>

const size_t testSize = 1000;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::milli;

void function3()
{
	throw;
}


std::string function1(int i)
{
	function3();

	if (i < 100)
	{
		return std::string("Hello");
	}

	return std::string("Hi");
}



double function2( int i)
{
	std::string str = function1(i);
	
	if (i < 200)
	{
		return 3.98;
	}

	return 9.45;
}

int init_number(int i)
{
	function2(i);

	if (i < 1000)
	{
		i = i;
	}
	else
	{
		i = i * 2;
	}

	return i;
}

int main()
{

	std::vector<int> ints(testSize);
	for (size_t i = 0; i < testSize; i++)
	{
		ints[i] = init_number(i);
		std::cout << init_number(i) << " " << function1(i) << " " << function2(i);
	}

	return 0;
}