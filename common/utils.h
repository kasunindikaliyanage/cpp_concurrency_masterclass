#pragma once
#include <stddef.h>
#include <stdio.h>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::milli;
using std::random_device;
using std::sort;
using std::vector;


void print_results(const char* const tag, high_resolution_clock::time_point startTime, high_resolution_clock::time_point endTime);

unsigned long get_optimal_num_threads();