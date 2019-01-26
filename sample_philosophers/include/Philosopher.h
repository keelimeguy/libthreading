#pragma once

#include <iostream>
#include <unistd.h>

#include "Core.h"
#include "Thread.h"
#include "Fork.h"
#include "Screen.h"

#define DATA_HEIGHT 40 // Show 40 philosopher entries per column
#define LINESTART 6 // Row offset, due to argument printout and etc

void philospher_simulation(int num_philosophers, int min_time, int max_time, int duration, bool use_center);

// Abstracts functions of philosopher and handles functions in own thread
class Philosopher {
public:
    Philosopher(int id, Fork* left, Fork* right, int think_time_us, int eat_time_us)
        : m_id(id), m_LeftFork(left), m_RightFork(right), m_uThinkTime(think_time_us), m_uEatTime(eat_time_us)
    {
        { // We need to print in critical sections so to not overlap print output
            ScopedMutex guard(Screen::lock);
            if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                std::cout << "Philosopher " << m_id << " is invited (think:"
                    << think_time_us << ",eat:" << eat_time_us << ")..\n";
            } else {
                // Write the state transition at the correct position in the table
                Screen::WriteAt(m_id%DATA_HEIGHT + LINESTART+4, 17*(int)(m_id/DATA_HEIGHT), "%d: invited ", m_id);
            }
        }

        // Begin the philosopher thread
        m_Thread = Core::MakeThread<void,Philosopher*>(-1, philosopher_task, this);
    }

    Thread<void,Philosopher*>* GetThread() { return m_Thread.get(); }
    inline int GetId() { return m_id; }

    // Check whether thread needs to exit (exit if not present)
    inline bool IsPresent() { return m_Present; }

    // Trigger the thread to exit
    void Leave() {
        m_Present = false;
    }

    // Tell the application to allow use of center fork
    static void UseCenter() { s_UseCenter = true; };

private:
    // Create a center fork that any philosopher can use in replacement of their own fork
    static Fork s_CenterFork;
    static bool s_UseCenter; // (only used if enabled)

    int m_id;
    Fork *m_LeftFork, *m_RightFork;
    int m_uThinkTime, m_uEatTime; // How long it takes to think/eat

    bool m_Present = true; // If the philosopher is present we will keep running the thread
    std::shared_ptr<Thread<void,Philosopher*>> m_Thread;

    void think() {
        {
            ScopedMutex guard(Screen::lock);
            if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                std::cout << "Philosopher " << m_id << " is thinking..\n";
            } else {
                // Write the state transition at the correct position in the table
                Screen::WriteAt(m_id%DATA_HEIGHT + LINESTART+4, 17*(int)(m_id/DATA_HEIGHT), "%d: thinking", m_id);
            }
        }
        // Simply wait a given amount of time for thinking
        usleep(m_uThinkTime);
    }

    void eat() {
        {
            ScopedMutex guard(Screen::lock);
            if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                std::cout << "Philosopher " << m_id << " is hungry..\n";
            } else {
                // Write the state transition at the correct position in the table
                Screen::WriteAt(m_id%DATA_HEIGHT + LINESTART+4, 17*(int)(m_id/DATA_HEIGHT), "%d: hungry  ", m_id);
            }
        }

        for (;;) { // Try to pick up forks forever (and end after successful)

            // Try the left fork
            ChooseFork left(m_LeftFork, m_id);

            // Try the right fork
            ChooseFork right(m_RightFork, m_id);

            // We can only eat if we have two forks
            bool can_eat = left.IsHeld() && right.IsHeld();

            // Try the center fork if enabled and we managed to pickup one fork
            bool try_center = s_UseCenter && !can_eat && (left.IsHeld() || right.IsHeld());

            // We use this inline if in order to keep "center" in the current scope until finished
            ChooseFork center(try_center ? &s_CenterFork : nullptr, m_id);
            if (try_center) can_eat = center.IsHeld();

            if (s_UseCenter && !Screen::IsVerbose()) { // Print the holder of center fork, for record keeping
                ScopedMutex guard(Screen::lock);
                Screen::WriteAt(LINESTART, 0, "(center holder: %d)\t\t\t\t\t\t", s_CenterFork.Holder());
            }

            // If we have two forks, then finally eat
            if (can_eat) {
                {
                    ScopedMutex guard(Screen::lock);
                    if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                        std::cout << "Philosopher " << m_id << " is eating..\n";
                    } else {
                        // Write the state transition at the correct position in the table
                        Screen::WriteAt(m_id%DATA_HEIGHT + LINESTART+4, 17*(int)(m_id/DATA_HEIGHT), "%d: eating  ", m_id);
                    }
                }

                // Wait a given amount of time for eating
                usleep(m_uEatTime);

                {
                    ScopedMutex guard(Screen::lock);
                    if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                        std::cout << "Philosopher " << m_id << " is full..\n";
                    } else {
                       // Write the state transition at the correct position in the table
                        Screen::WriteAt(m_id%DATA_HEIGHT + LINESTART+4, 17*(int)(m_id/DATA_HEIGHT), "%d: full    ", m_id);
                    }
                }

                // Break from the loop since we just ate
                return;
            }
        }
    }

    // Define the thread function:
    // --  void* philosopher_task(Philosopher** arg)
    static THREAD_FUNC(philosopher_task, void,Philosopher*) {
        {
            ScopedMutex guard(Screen::lock);
            if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                std::cout << "Philosopher " << (*arg)->GetId() << " has arrived..\n";
            } else {
                // Write the state transition at the correct position in the table
                Screen::WriteAt((*arg)->GetId()%DATA_HEIGHT + LINESTART+4, 17*(int)((*arg)->GetId()/DATA_HEIGHT), "%d: arrived ", (*arg)->GetId());
            }
        }

        // Perform tasks until triggered to stop
        while ((*arg)->IsPresent()) {
            // Alternate between thinking and eating
            (*arg)->think();
            if (!(*arg)->IsPresent()) break; // Don't bother eating if need to leave
            (*arg)->eat();
        }

        {
            ScopedMutex guard(Screen::lock);
            if (Screen::IsVerbose()) { // If not using fancy visualization then just print the state transition
                std::cout << "Philosopher " << (*arg)->GetId() << " is leaving..\n";
            } else {
                // Write the state transition at the correct position in the table
                Screen::WriteAt((*arg)->GetId()%DATA_HEIGHT + LINESTART+4, 17*(int)((*arg)->GetId()/DATA_HEIGHT), "%d: leaving ", (*arg)->GetId());
            }
        }

        THREAD_RETURN(nullptr);
    }
};
