#pragma once

#include <thread>
#include <numeric>
#include <algorithm>
#include <vector>
#include <functional>

template<typename iterator, typename T>
void accumulate(iterator first, iterator last, T& val)
{
	val = std::accumulate(first, last, val);
}

template<typename iterator, typename T>
void parallel_accumulate(iterator start, iterator end, T& ref)
{
	unsigned MIN_BLOCK_SIZE = 1000;

	unsigned distance = std::distance(start, end);
	unsigned allowed_threads_by_elements = (distance + 1) / MIN_BLOCK_SIZE;
	unsigned allowed_threads_by_hardware = std::thread::hardware_concurrency();

	// this is due to the fact that some operating system may 
	// return 0 to indicate that this has not implemented
	if (allowed_threads_by_hardware < 1)
		allowed_threads_by_hardware = 2;

	unsigned allowed_threads = std::min(allowed_threads_by_elements,
		allowed_threads_by_hardware);

	//caculating the block size 
	unsigned block_size = (distance + 1) / allowed_threads;

	std::vector<T> results(allowed_threads);
	std::vector<std::thread> threads(allowed_threads - 1);

	//iterate and craeting new threads to calculate sum for each blocks
	iterator last;
	for (unsigned i = 0; i < allowed_threads - 1; i++)
	{
		last = start;
		std::advance(last, block_size);
		threads[i] = std::thread(accumulate<iterator, T>, start, last,
			std::ref(results[i]));
		start = last;
	}

	//final block will be calculated from this thread
	results[allowed_threads - 1] =
		std::accumulate(start, end, results[allowed_threads - 1]);

	//calling join on the newly craeted threads
	for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	ref = std::accumulate(results.begin(), results.end(), ref);
}