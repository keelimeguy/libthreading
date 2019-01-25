/*=============================================================================
    Copyright (c) 2019 Keelin Becker-Wheeler
    Core.h
    Distributed under the GNU GENERAL PUBLIC LICENSE
    See https://github.com/keelimeguy/libthreading
==============================================================================*/
#pragma once

#include <unistd.h>
#include <cassert>
#include <memory>
#include <vector>

#include "Thread.h"

// Allows a thread to be created on particular a cpu
class Core {
public:
    // WARNING: you MUST store the return variable of MakeThread(..) in a variable,
    // or else the constructed argument will go out of scope and be deleted!
    // e.g. auto t = Core::MakeThread<void,int>(cpu, func, i);

    static void Init() {
        // Store the total number of parallel resources
        m_numProc = sysconf(_SC_NPROCESSORS_ONLN);
        assert(m_numProc);

        // Extract the actual CPUs available for this process
        cpu_set_t cpus;
        CPU_ZERO(&cpus); // Start with zero set (no CPUs)
        // First get the affinity of the current process
        sched_getaffinity(0, sizeof(cpu_set_t), &cpus);
        // Then for each of the parallel resources in total,
        // check if that resource is available based on the affinity
        for (int i = 0; i < (int)m_numProc; ++i)
            if(CPU_ISSET(i, &cpus)) m_avail.push_back(i);
        assert(m_avail.size());
    }

    static inline unsigned int Count() { return m_avail.size(); }
    static inline unsigned int NumProc() { return m_numProc; }

    // Create a Thread (see Thread.h) on a particular CPU
    template<typename Ret, typename Arg> // Take Arg directly
    static std::shared_ptr<Thread<Ret,Arg>> MakeThread(int cpu, void*(*task)(void*), Arg& arg) {
        assert(cpu >= -1 && cpu < (int)Count());
        return std::make_shared<Thread<Ret,Arg>>((cpu >= 0) ? m_avail[cpu] : cpu, task, arg);
    }

    // Create a Thread (see Thread.h) on a particular CPU
    template<typename Ret, typename Arg, typename ... Args> // Construct Arg indirectly with Args
    static std::shared_ptr<Thread<Ret,Arg>> MakeThread(int cpu, void*(*task)(void*), Args&& ... args) {
        assert(cpu >= -1 && cpu < (int)Count());
        return std::make_shared<Thread<Ret,Arg>>((cpu >= 0) ? m_avail[cpu] : cpu, task, std::forward<Args>(args) ...);
    }

private:
    static unsigned int m_numProc;
    static std::vector<int> m_avail;
};
