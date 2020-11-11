#include <iostream>
#include <thread>
#include <chrono>

void func_1()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "hello from func_1 \n";
}


int main()
{
	std::thread thread_1(func_1);
	thread_1.join();        // first example code. Comment this for second example
	// thread_1.detach();   // second example code. Comment this for first example

	std::cout << "hello from main thread \n";
}