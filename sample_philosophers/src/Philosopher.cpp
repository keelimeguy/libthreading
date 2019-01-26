#include "Philosopher.h"
#include "Fork.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

// Allocate static class variables
Fork Philosopher::s_CenterFork;
bool Philosopher::s_UseCenter = false;

void philospher_simulation(int num_philosophers, int min_time, int max_time, int duration, bool use_center) {
    // Check for valid arguments
    if (num_philosophers <= 0 || min_time <= 0 || max_time <= 0 || duration < 0) return;

    srand(time(0)); // Seed RNG

    // Allocate enough forks
    int num_forks = (num_philosophers == 1) ? 2 : num_philosophers;
    std::vector<Fork> forks(num_forks);

    // Allow use of center fork if requested
    if (use_center) Philosopher::UseCenter();

    // If no duration given then we will end program on user key input
    if (!duration) {
        Screen::WriteAt(LINESTART, 0, "\n[Type a key to exit the program.]\n\n");
        if (Screen::IsVerbose())
            sleep(3); // Wait a bit to make sure exit condition is read by user
    } else {
        Screen::WriteAt(LINESTART, 0, "\n[Terminating after %d seconds]\n\n", duration);
    }

    Screen::WriteAt(LINESTART+2, 0, "Inviting %d philosophers..\n", num_philosophers);

    // Helper variables to assign forks to philosophers
    Fork *left, *right = &forks[num_forks-1];

    // Allocate philosophers
    std::vector<std::shared_ptr<Philosopher>> philosophers(num_philosophers);

    for (int i = 0; i < num_philosophers; ++i) {
        // Create thinking and eating durations, based on argument bounds
        int think_time_us = rand() % (max_time-min_time+1) + min_time;
        int eat_time_us = rand() % (max_time-min_time+1) + min_time;

        left = &forks[i]; // Assign left fork

        // Create the philosopher thread
        philosophers[i] = std::make_shared<Philosopher>(i, left, right, think_time_us, eat_time_us);

        right = left; // Rotate forks to share between neighboring philosophers
    }

    // Wait given amount of time before exiting
    if (duration) sleep(duration);

    // Wait on user input to exit program
    else std::cin.get();

    // End threads before exiting
    for (auto& philosopher : philosophers)
        philosopher->Leave();
    for (auto& philosopher : philosophers)
        philosopher->GetThread()->Join();
}
