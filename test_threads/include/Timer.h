#pragma once

#include <chrono>
using Clock=std::chrono::high_resolution_clock;

// Wraps chrono timing mechanisms for convenience
namespace Timer {
    static Clock::time_point m_start;

    static void Start() {
        m_start = Clock::now();
    }

    // Returns ellapsed seconds since Start() was called
    static double EllapsedSec() {
        auto end = Clock::now();
        double seconds = (std::chrono::duration_cast<std::chrono::duration<double>>(end - m_start)).count();
        return seconds;
    }
}
