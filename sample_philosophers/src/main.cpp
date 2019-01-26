#include <iostream>

#include <args.hpp>
#include "cli.h"

#include "Core.h"
#include "Screen.h"
#include "Timer.h"
#include "Philosopher.h"

int main(int argc, char const *argv[]) {

    // Parse the command line arguments
    args::parse<cli>(argc, argv);
    if (!cli::valid) {
        std::cout << "Invalid args. Try: " << argv[0] << " -h\n";
        return 0;
    }

    // Initialize the screen for either fancy table output (!verbose), or list of transitions (verbose)
    Screen::Init(cli::verbose);
    Screen::Write("");

    if (!cli::verbose) { // Print the arguments above fancy table output, if enabled
        Screen::Write("Args:   verbose=%s   num_philosophers=%d   min_time=%d   max_time=%d   use_center=%s   duration=%d",
        (cli::verbose?"true":"false"), cli::num_philosophers, cli::min_time, cli::max_time, (cli::use_center?"true":"false"), cli::duration);
        Screen::Write("");
    }

    // Initialize core so that we can verify the number of system resources
    Core::Init(); // (only needed for that purpose)

    // Record the system resources for record keeping
    Screen::Write("available CPUs for use: %d\n", Core::Count());
    Screen::Write("(this system has a total of %d possible CPUs)\n", Core::NumProc());

    // Run the philosopher problem simulation
    Timer::Start();
    philospher_simulation(cli::num_philosophers, cli::min_time, cli::max_time, cli::duration, cli::use_center);
    double s = Timer::EllapsedSec();

    Screen::Terminate();

    // Record the time taken for the fun of it!
    std::cout << "\nFinished after " << s << "s\n";

    return 0;
}
