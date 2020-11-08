#pragma once

#include <vector>
#include <thread>
#include <future>
#include <atomic>

#include "common_objs.h"

template<typename Iterator, typename MatchType>
Iterator parallel_find_pt(Iterator first, Iterator last, MatchType match)
{
	struct find_element 
	{
		void operator()(Iterator begin, Iterator end,
			MatchType match,
			std::promise<Iterator>* result,
			std::atomic<bool>* done_flag)
		{
			try
			{
				for( ; (begin != end) && !std::atomic_load(done_flag); ++begin)
				{
					if (*begin == match)
					{
						result->set_value(begin);
						//done_flag.store(true);
						std::atomic_store(done_flag, true);
						return;
					}
				}
			}
			catch (...)
			{
				result->set_exception(std::current_exception());
				done_flag->store(true);
			}
		}
	};

	unsigned long const length = std::distance(first, last);

	if (!length)
		return last;

	/*	Calculate the optimized number of threads to run the algorithm	*/

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;

	/*	Declare the needed data structures	*/
	std::promise<Iterator> result;
	std::atomic<bool> done_flag(false);
	std::vector<std::thread> threads(num_threads - 1);
	{
		join_threads joiner(threads);

		Iterator block_start = first;
		for (unsigned long i = 0; i < (num_threads - 1); i++)
		{
			Iterator block_end = block_start;
			std::advance(block_end, block_size);

			threads[i] = std::thread(find_element(), block_start, block_end, match, &result, &done_flag);

			block_start = block_end;
		}

		// perform the find operation for final block in this thread.
		find_element()(block_start, last, match, &result, &done_flag);
	}

	if (!done_flag.load())
	{
		return last;
	}

	return result.get_future().get();
}

template<typename Iterator, typename MatchType>
Iterator parallel_find_async(Iterator first, Iterator last, MatchType match, std::atomic<bool>* done_flag)
{
	try
	{
		unsigned long const length = std::distance(first, last);
		unsigned long const min_per_thread = 25;

		if (length < 2 * min_per_thread)
		{
			for ( ; (first != last ) && done_flag; ++first)
			{
				if (*first == match)
				{
					*done_flag = true;
					return first;
				}
			}
			return last;
		}
		else
		{
			Iterator const mid_point = first + length / 2;
			std::future<Iterator> async_result =
				std::async(&parallel_find_async<Iterator, MatchType>,mid_point, last, match, std::ref(done_flag));

			Iterator const direct_result =
				parallel_find_async(first, mid_point, match, done_flag);

			return (direct_result == mid_point) ? async_result.get() : direct_result;
		}
	}
	catch (const std::exception&)
	{
		*done_flag = true;
		throw;
	}
}