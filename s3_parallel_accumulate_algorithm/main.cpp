#include <iostream>
#include <future>
#include <numeric>

int MIN_ELEMENT_COUNT = 1000;

template<typename iterator>
int parallal_accumulate(iterator begin, iterator end)
{
	long length = std::distance(begin, end);

	//atleast runs 1000 element
	if (length <= MIN_ELEMENT_COUNT)
	{
		std::cout << std::this_thread::get_id() << std::endl;
		return std::accumulate(begin, end, 0);
	}

	iterator mid = begin;
	std::advance(mid, (length + 1) / 2);

	//recursive all to parallel_accumulate
	std::future<int> f1 = std::async(std::launch::deferred | std::launch::async,
		parallal_accumulate<iterator>, mid, end);
	auto sum = parallal_accumulate(begin, mid);
	return sum + f1.get();
}

int main()
{
	std::vector<int> v(10000, 1);
	std::cout << "The sum is " << parallal_accumulate(v.begin(), v.end()) << '\n';
}