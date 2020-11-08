#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/bind.hpp>
#include <boost/atomic.hpp>
#include <iostream>

void func(boost::barrier& cur_barier, boost::atomic<int>& counter)
{
    ++counter;
    //std::cout  << counter << "\n";
    cur_barier.wait();
    std::cout << "Current value after barrier :" << counter << "\n";
}

int main()
{
    boost::barrier bar(3);
    boost::atomic<int> current(0);
    boost::thread thr1(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr2(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr3(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr4(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr5(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    boost::thread thr6(boost::bind(&func, boost::ref(bar), boost::ref(current)));
    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
}