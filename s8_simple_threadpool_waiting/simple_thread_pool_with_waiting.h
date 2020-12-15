#pragma once

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <future>
#include <numeric>
#include "common_thread_safe_queue.h"
#include "common_objs.h"

#include "utils.h"

class thread_pool_waiting {

	std::atomic_bool done;
	threadsafe_queue< function_wrapper> work_queue;
	std::vector<std::thread> threads;
	join_threads joiner;

	void worker_thread()
	{
		while (!done)
		{
			function_wrapper task;
			if (work_queue.try_pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

public:
	thread_pool_waiting() :done(false), joiner(threads)
	{
		int const thread_count = std::thread::hardware_concurrency();

		try
		{
			for (int i = 0; i < thread_count; ++i)
			{
				threads.push_back(
					std::thread(&thread_pool_waiting::worker_thread, this));
			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}

	~thread_pool_waiting()
	{
		done = true;
	}

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type>
		submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type result_type;
		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
	}
};

template<typename Iterator, typename T>
struct accumulate_block
{
	T operator()(Iterator first, Iterator last)
	{
		T value = std::accumulate(first, last, T());
		printf(" %d - %d  \n", std::this_thread::get_id(), value);
		return value;
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);
	thread_pool_waiting pool;

	if (!length)
		return init;

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads =
		(length + min_per_thread - 1) / min_per_thread;

	unsigned long const hardware_threads =
		std::thread::hardware_concurrency();

	unsigned long const num_threads =
		std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

	unsigned long const block_size = length / num_threads;

	std::vector<std::future<T> > futures(num_threads - 1);

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		futures[i] = pool.submit(std::bind(accumulate_block<Iterator, T>(), block_start, block_end));
		block_start = block_end;
	}
	T last_result = accumulate_block<int*, int>()(block_start, last);

	T result = init;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		result += futures[i].get();
	}
	result += last_result;
	return result;
}


