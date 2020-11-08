// Type your code here, or load an example.
#include <thread>

#include <chrono>
#include <iostream>
#include "jthread.h"
//
using namespace::std::literals;
//
//void do_something(std::stop_token token)
//{
//    int counter{ 0 };
//    while (counter < 10)
//    {
//        if (token.stop_requested())
//        {
//            return;
//        }
//        std::this_thread::sleep_for(0.2s);
//        std::cerr << "This is interruptible thread : " << counter << std::endl;
//        ++counter;
//    }
//}
//
//void do_something_else()
//{
//    int counter{ 0 };
//    while (counter < 10)
//    {
//        std::this_thread::sleep_for(0.2s);
//        std::cerr << "This is non-interruptible thread : " << counter << std::endl;
//        ++counter;
//    }
//}
//
//int main() {
//
//    std::cout << std::endl;
//    std::jthread nonInterruptable(do_something_else);
//    std::jthread interruptible(do_something);
//
//    std::this_thread::sleep_for(1.0s);
//    interruptible.request_stop();
//    nonInterruptable.request_stop();
//
//    std::cout << std::endl;
//
//}

void do_something()
{
    int counter{ 0 };
    while (counter < 10)
    {
        if (interrupt_point())
        {
            return;
        }
        std::this_thread::sleep_for(0.2s);
        std::cerr << "This is interruptible thread : " << counter << std::endl;
        ++counter;
    }
}

void do_something_else()
{
    int counter{ 0 };
    while (counter < 10)
    {
        std::this_thread::sleep_for(0.2s);
        std::cerr << "This is non-interruptible thread : " << counter << std::endl;
        ++counter;
    }
}

int main()
{

    std::cout << std::endl;
    jthread_local nonInterruptable(do_something_else);
    jthread_local interruptible(do_something);

    std::this_thread::sleep_for(1.0s);
    interruptible.interrupt();
    nonInterruptable.interrupt();

    //std::cout << std::endl;
	return 0;
}