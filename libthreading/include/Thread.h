#pragma once

#include <cassert>
#include <memory>
#include <pthread.h>

#define THREAD_FUNC(func, Ret, Arg) void* func(void* args) { Arg *arg = (Arg*) args;
#define THREAD_RETURN(ret) pthread_exit((void*)ret); } return nullptr
#define MAIN_THREAD_RETURN() pthread_exit(nullptr)

// WARNING: if a Thread object ever goes out of scope,
// the constructed argument is deleted and a running
// thread may not work as expected.
// i.e. don't let Thread objects go out of scope before their completion


// Wraps pthread_t for convenience
// - Ret is the return type of wrapped thread task
// - Arg is the input argument of wrapped thread task
template<typename Ret, class Arg>
class Thread {
public:
    Thread(void*(*task)(void*), Arg& arg);
    Thread(int cpu, void*(*task)(void*), Arg& arg);

    template<typename ... Args>
    Thread(void*(*task)(void*), Args&& ... args);

    template<typename ... Args>
    Thread(int cpu, void*(*task)(void*), Args&& ... args);

    Ret* Join();
    bool ResultPending();

private:
    Ret* m_ret;
    bool m_running;
    int m_cpu;
    void*(*m_task)(void*);
    pthread_t m_thread;
    std::shared_ptr<Arg> m_arg;

    void create();
};

// Constructs thread from task and argument of necessary input type
template<typename Ret, class Arg>
Thread<Ret,Arg>::Thread(void*(*task)(void*), Arg& arg)
    : m_ret(nullptr), m_running(false), m_cpu(-1), m_task(task)
{
    m_arg = std::make_shared<Arg>(arg);
    create();
}

template<typename Ret, class Arg>
Thread<Ret,Arg>::Thread(int cpu, void*(*task)(void*), Arg& arg)
    : m_ret(nullptr), m_running(false), m_cpu(cpu), m_task(task)
{
    m_arg = std::make_shared<Arg>(arg);
    create();
}

// Constructs thread from task and variable arguments to construct necessary input type
// knowledge from-> https://stackoverflow.com/questions/40003796/pass-variable-argument-list-to-base-constructor-in-c
template<typename Ret, class Arg>
template<typename ... Args>
Thread<Ret,Arg>::Thread(void*(*task)(void*), Args&& ... args)
    : m_ret(nullptr), m_running(false), m_cpu(-1), m_task(task)
{
    m_arg = std::make_shared<Arg>(std::forward<Args>(args) ...);
    create();
}

template<typename Ret, class Arg>
template<typename ... Args>
Thread<Ret,Arg>::Thread(int cpu, void*(*task)(void*), Args&& ... args)
    : m_ret(nullptr), m_running(false), m_cpu(cpu), m_task(task)
{
    m_arg = std::make_shared<Arg>(std::forward<Args>(args) ...);
    create();
}

// Create the pthread object
template<typename Ret, class Arg>
void Thread<Ret,Arg>::create() {
    pthread_attr_t attr;

    // Explicitly set thread as joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (m_cpu >= 0) {
        // Set thread to run on a particular cpu
        cpu_set_t cpus;
        CPU_ZERO(&cpus);
        CPU_SET(m_cpu, &cpus);
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
    }

    // Create the thread and terminate on an error
    int err = pthread_create(&m_thread, &attr, m_task, (void*) m_arg.get());
    assert(!err);

    m_running = true;

    pthread_attr_destroy(&attr);
}

// Wait for thread to terminate and pass return value
template<typename Ret, class Arg>
Ret* Thread<Ret,Arg>::Join() {
    if (m_running) {
        void *status;
        int err = pthread_join(m_thread, &status);
        assert(!err);

        m_running = false;
        m_ret = (Ret*) status;
    }
    return m_ret;
}

template<typename Ret, class Arg>
bool Thread<Ret,Arg>::ResultPending() {
    if (m_running && !pthread_tryjoin_np(m_thread, (void**)(&m_ret))) {
        m_running = false;
        return true;
    }
    return false;
}

// Specialization for void argument
template<typename Ret>
class Thread<Ret,void> {
public:
    Thread(void*(*task)(void*))
        : m_ret(nullptr), m_running(false), m_cpu(-1), m_task(task)
    { create(); }

    Thread(int cpu, void*(*task)(void*))
        : m_ret(nullptr), m_running(false), m_cpu(cpu), m_task(task)
    { create(); }

    Ret* Join();
    bool TryJoin(Ret** ret);

private:
    Ret* m_ret;
    bool m_running = false;
    int m_cpu;
    void*(*m_task)(void*);
    pthread_t m_thread;

    void create();
};

template<typename Ret>
void Thread<Ret,void>::create() {
    pthread_attr_t attr;

    // Explicitly set thread as joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (m_cpu >= 0) {
        // Set thread to run on a particular cpu
        cpu_set_t cpus;
        CPU_ZERO(&cpus);
        CPU_SET(m_cpu, &cpus);
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);
    }

    // Create the thread and terminate on an error
    int err = pthread_create(&m_thread, &attr, m_task, nullptr);
    assert(!err);

    m_running = true;

    pthread_attr_destroy(&attr);
}
