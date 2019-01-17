#pragma once

#include <iostream>

// The command line interface,
// based on examples from-> https://github.com/pfultz2/args

// // Parse command line arguments
// args::parse<cli>(argc, argv);
// if (!cli::valid) return 0;

struct cli {
    static const char* help() {
        return "Find sum of array of numbers";
    }
    static const char* name() { return "sum"; }

    static int size;
    static int max;
    static int num_threads;
    static bool verbose;
    static bool valid;

    template<class F>
    void parse(F f) {
        f(verbose, "--verbose", "-v");
        f(size, "--size", "-n", args::help("The size of the random array. (default=1000000)"));
        f(max, "--max", "-M", args::help("The maximum value in the random array. (default=10)"));
        f(num_threads, "--num_threads", "-t", args::help("The number of threads. (default=8)"));
    }

    void run() {
        valid = true;

        // Fixes the odd behavior of the vendor library,
        // e.g. so that now the flag -v results in verbose=true (else false without flag use)
        verbose = !verbose;

        // Report arguments, for benefit of record keeping
        std::cout << "Args:\tsize=" << size
            << "\n\tmax=" << max
            << "\n\tnum_threads=" << num_threads
            << "\n\tverbose=" << (verbose?"true":"false") << std::endl;
    }
};

bool cli::valid = false;

// Default values:
int cli::size = 1000000;
int cli::max = 10;
int cli::num_threads = 8;
// Due to how the args library works these are opposite valued..
bool cli::verbose = true;
