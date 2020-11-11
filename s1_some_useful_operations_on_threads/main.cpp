#include <iostream>
#include <thread>
#include <chrono>

void run_code1()
{
	std::cout << "Allowed max number of parallel threads : "
		<< std::thread::hardware_concurrency() << std::endl;
}


void func_1()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "new  thread id : " << std::this_thread::get_id() << std::endl;
}

void run_code2()
{
	std::thread thread_1(func_1);

	std::cout << "thread_1 id before joining : " << thread_1.get_id() << std::endl;
	thread_1.join();

	std::thread thread_2;

	std::cout << "default consturcted thread id : " << thread_2.get_id() << std::endl;
	std::cout << "thread_1 id after joining : " << thread_1.get_id() << std::endl;
	std::cout << "Main thread id : " << std::this_thread::get_id() << std::endl;

	std::cout << "\n\nAllowed max number of parallel threads : "
		<< std::thread::hardware_concurrency() << std::endl;
}

int main()
{
	run_code1();	 // example 1
	//run_code2();	 // example 2
}