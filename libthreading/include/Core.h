#pragma once

#include <unistd.h>
#include <cassert>
#include <memory>
#include <vector>

#include "Thread.h"

class Core {
public:
    // WARNING: you MUST store the return variable of MakeThread(..) in a variable,
    // or else the constructed argument will go out of scope and be deleted!
    // e.g. auto t = core.Run<void,int>(func, i);

    static void Init() {
        m_numProc = sysconf(_SC_NPROCESSORS_ONLN);
        assert(m_numProc);

        cpu_set_t cpus;
        CPU_ZERO(&cpus);
        sched_getaffinity(0, sizeof(cpu_set_t), &cpus);
        for (int i = 0; i < m_numProc; ++i)
            if(CPU_ISSET(i, &cpus)) m_avail.push_back(i);
        assert(m_avail.size());
    }

    static inline unsigned int Count() { return m_avail.size(); }
    static inline unsigned int NumProc() { return m_numProc; }

    template<typename Ret, typename Arg>
    static std::shared_ptr<Thread<Ret,Arg>> MakeThread(int cpu, void*(*task)(void*), Arg& arg) {
        assert(cpu >= -1 && cpu < (int)Count());
        return std::make_shared<Thread<Ret,Arg>>((cpu >= 0) ? m_avail[cpu] : cpu, task, arg);
    }

    template<typename Ret, typename Arg, typename ... Args>
    static std::shared_ptr<Thread<Ret,Arg>> MakeThread(int cpu, void*(*task)(void*), Args&& ... args) {
        assert(cpu >= -1 && cpu < (int)Count());
        return std::make_shared<Thread<Ret,Arg>>((cpu >= 0) ? m_avail[cpu] : cpu, task, std::forward<Args>(args) ...);
    }

private:
    static unsigned int m_numProc;
    static std::vector<int> m_avail;
};
