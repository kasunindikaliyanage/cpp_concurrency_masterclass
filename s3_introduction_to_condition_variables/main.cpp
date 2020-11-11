#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <thread>
#include <chrono>

bool have_i_arrived = false;
int distance_my_destination = 10;
int distance_coverd = 0;

bool keep_driving()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		distance_coverd++;
	}

	return false;
}

void keep_awake_all_night()
{
	while (distance_coverd < distance_my_destination)
	{
		std::cout << "keep check, whether i am there \n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cout << "finally i am there, distance_coverd = " << distance_coverd << std::endl;;
}

void set_the_alarm_and_take_a_nap()
{
	if (distance_coverd < distance_my_destination)
	{
		std::cout << "let me take a nap \n";
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
	std::cout << "finally i am there, distance_coverd = " << distance_coverd << std::endl;;
}

int main()
{
	std::thread driver_thread(keep_driving);
	std::thread keep_awake_all_night_thread(keep_awake_all_night);
	std::thread set_the_alarm_and_take_a_nap_thread(set_the_alarm_and_take_a_nap);

	keep_awake_all_night_thread.join();
	set_the_alarm_and_take_a_nap_thread.join();
	driver_thread.join();
}
