#include <iostream>
#include <thread>

#include "thread_safe_queue.h"

thread_safe_queue<int> queue;

void func_1()
{
	int value;
	queue.wait_pop(value);
	std::cout << value << std::endl;

}

void func_2()
{
	int x = 10;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	queue.push(x);
}

int main()
{
	std::thread thread_1(func_1);
	std::thread thread_2(func_2);

	thread_1.join();
	thread_2.join();
}