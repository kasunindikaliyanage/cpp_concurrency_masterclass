#include "utils.h"
#include "common_objs.h"

void print_results(const char* const tag,
    high_resolution_clock::time_point startTime,
    high_resolution_clock::time_point endTime) {
    printf("%s: Time: %fms\n", tag,
        duration_cast<duration<double, milli>>(endTime - startTime).count());
}