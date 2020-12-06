#pragma once

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <functional>
#include "common_thread_safe_queue.h"
#include "common_objs.h"

#include "utils.h"

class thread_pool
{
	std::atomic_bool done;
	threadsafe_queue<std::function<void()>> work_queue;
	std::vector<std::thread> threads;
	join_threads joiner;

	void worker_thread()
	{
		while (!done)
		{
			std::function<void()> task;
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
	thread_pool() :done(false), joiner(threads)
	{
		int const thread_count = std::thread::hardware_concurrency();

		try
		{
			for (int i = 0; i < thread_count; i++)
			{
				threads.push_back(
					std::thread(&thread_pool::worker_thread, this));
			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}

	~thread_pool()
	{
		done = true;
	}

	template<typename Function_type>
	void submit(Function_type f)
	{
		work_queue.push(std::function<void()>(f));
	}
};