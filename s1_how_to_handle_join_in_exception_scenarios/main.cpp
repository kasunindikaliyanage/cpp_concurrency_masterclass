#include <iostream>
#include <thread>
#include <stdexcept>
#include <chrono>
#include "common_objs.h"

void func_1()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "hello from method \n";
}

void other_operations()
{
	std::cout << "This is other operation \n";
	throw std::runtime_error("this is a runtime error");
}

/////////////////////////////// for first example
void run_code1()
{
	std::thread thread_1(func_1);

	try {
		//do other operations
		other_operations();
		thread_1.join();
	}
	catch (...)
	{
	}
	std::cout << "This is main thread \n";
}

/////////////////////////////// for second example
void run_code2()
{
	std::thread thread_1(func_1);

	//do other operations
	try
	{
		other_operations();
		thread_1.join();
	}
	catch (...)
	{
		thread_1.join();
	}
}

/////////////////////////////// for third example
void run_code3()
{
	std::thread thread_1(func_1);
	thread_guard tg(thread_1);

	//do other operations
	try
	{
		other_operations();
	}
	catch (...)
	{
	}
}


int main()
{
	run_code1();
	//run_code2();
	//run_code3();
}