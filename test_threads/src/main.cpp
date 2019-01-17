#include <cstdlib>
#include <iostream>
#include <vector>

#include <args.hpp>
#include "cli.h"

#include "par_sum.h"
#include "Core.h"
#include "Timer.h"

void print_array(int *arr, int size);

int main(int argc, char const *argv[]) {
    args::parse<cli>(argc, argv);
    if (!cli::valid) return 0;

    Timer::Start();
    Core::Init();

    std::vector<int> nums(cli::size);
    for (int i = 0; i < cli::size; ++i)
        nums[i] = rand() % (cli::max + 1);

    if (cli::verbose) print_array(&nums[0], cli::size);

    long sum;

    std::cout << "avail threads: " << Core::Count() << std::endl;

    Timer::Start();
    sum = par_sum(&nums[0], cli::size, cli::num_threads);
    double s = Timer::EllapsedSec();

    std::cout << sum << std::endl;
    std::cout << "\ntime: " << (s*1000) << "ms" << std::endl;

    return 0;
}

void print_array(int *arr, int size) {
    if (size <= 0) return;

    int i = 0;
    for(; i < size-1; ++i)
        std::cout << arr[i] << ",";
    std::cout << arr[i] << std::endl;
}
