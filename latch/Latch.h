#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>

class Latch {

	unsigned const count;
	std::atomic<unsigned> spaces;
	std::atomic<unsigned> generation;

public:
	explicit Latch(unsigned count_) :
		count(count_), spaces(count), generation(0)
	{}
	void wait()
	{
		unsigned const my_generation = generation;

		if (--spaces)
		{
			while (generation == my_generation)
				std::this_thread::yield();
		}
		else {
			generation++;
		}
	}
};