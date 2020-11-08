#include <iostream>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
#include <execution>

#include "partial_sum.h"
#include "utils.h"

const size_t testSize = 1'000;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::milli;

int main()
{
    std::vector<int> ints(testSize);
    std::vector<int> outs(testSize);
    for (auto& i : ints) {
        i = 1;
    }

    //sequential implementation
    auto startTime = high_resolution_clock::now();
    std::inclusive_scan(ints.cbegin(), ints.cend(), outs.begin());
    auto endTime = high_resolution_clock::now();
    print_results("sequential scan", startTime, endTime);

    // C++17 parallel implementation
    startTime = high_resolution_clock::now();
    std::inclusive_scan(std::execution::par,ints.cbegin(), ints.cend(), outs.begin());
    endTime = high_resolution_clock::now();
    print_results("parallel scan", startTime, endTime);

    // Our parallel implementation
    startTime = high_resolution_clock::now();
    parallel_partial_sum(ints.begin(), ints.end());
    endTime = high_resolution_clock::now();
    print_results("parallel scan manual", startTime, endTime);
   
    return 0;
}