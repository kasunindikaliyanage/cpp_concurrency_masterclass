#pragma once

#include <iostream>
#include <list>
#include <algorithm>
#include <future>


template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
	//std::cout << std::this_thread::get_id() << std::endl;
	if (input.size() < 2)
	{
		return input;
	}

	//move frist element in the list to result list and take it as pivot value
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T pivot = *result.begin();

	//partition the input array
	auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t)
		{
			return t < pivot;
		});

	//move lower part of the list to separate list so that we can make recursive call
	std::list<T> lower_list;
	lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

	auto new_lower(parallel_quick_sort(std::move(lower_list)));
	//auto new_upper(sequential_quick_sort(std::move(input)));
	std::future<std::list<T>> new_upper_future
	(std::async(&parallel_quick_sort<T>, std::move(input)));

	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_upper_future.get());
	return result;
}


template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	//recursive condition
	if ( input.size() < 2 )
	{
		return input;
	}

	//move frist element in the list to result list and take it as pivot value
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T pivot = *result.begin();

	//partition the input array so that t< pivot in lower part and t> pivot in upper part of input list
	auto divide_point = std::partition(input.begin(), input.end(),
		[&](T const& t)
		{
			return t < pivot;
		});

	//move lower part of the list to separate list so that we can make recursive call
	std::list<T> lower_list;
	lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

	//call the sequenctial_quick_sort recursively
	auto new_lower(sequential_quick_sort(std::move(lower_list)));
	auto new_upper(sequential_quick_sort(std::move(input)));

	//transfer all elements in to result list
	result.splice(result.begin(), new_lower);
	result.splice(result.end(), new_upper);

	return result;
}