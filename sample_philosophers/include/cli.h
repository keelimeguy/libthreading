#pragma once

#include <iostream>

// The command line interface,
// based on examples from-> https://github.com/pfultz2/args

// // Parse command line arguments
// args::parse<cli>(argc, argv);
// if (!cli::valid) return 0;

struct cli {
    static const char* help() {
        return "Run simulation of dining philosophers problem";
    }
    static const char* name() { return "philosophers"; }

    static int num_philosophers;
    static int min_time;
    static int max_time;
    static bool use_center;
    static int duration;
    static bool verbose;
    static bool valid;

    template<class F>
    void parse(F f) {
        f(verbose, "--verbose", "-v", args::help("Disable visual mode and print state verbosely."));
        f(num_philosophers, "--num_philosophers", "-n", args::help("The number of philosophers. (default=8)"));
        f(min_time, "--min_time", "-m", args::help("The minimum wait time (us) for thinking and eating. (default=1000000)"));
        f(max_time, "--max_time", "-M", args::help("The maximum wait time (us) for thinking and eating. (default=2000000)"));
        f(use_center, "--use_center", "-c", args::help("Enable option to use center fork."));
        f(duration, "--duration", "-t", args::help("Run for the given amount of seconds then exit. (default=0, user triggers exit)"));
    }

    void run() {
        valid = (num_philosophers>0 && min_time>0 && max_time>=min_time && duration>=0);

        // Fixes the odd behavior of the vendor library,
        // e.g. so that now the flag -v results in verbose=true (else false without flag use)
        verbose = !verbose;
        use_center = !use_center;

        // Report arguments, for benefit of record keeping
        std::cout << "Args:"
            << "\n\tverbose=" << (verbose?"true":"false")
            << "\n\tnum_philosophers=" << num_philosophers
            << "\n\tmin_time=" << min_time
            << "\n\tmax_time=" << max_time
            << "\n\tuse_center=" << (use_center?"true":"false")
            << "\n\tduration=" << duration << std::endl;
    }
};

bool cli::valid = false;

// Default values:
int cli::num_philosophers = 8;
int cli::min_time = 1000000;
int cli::max_time = 2000000;
int cli::duration = 0;
// Due to how the args library works these are opposite valued..
bool cli::verbose = true;
bool cli::use_center = true;
