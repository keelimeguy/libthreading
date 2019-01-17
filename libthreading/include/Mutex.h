#pragma once

#include <cassert>
#include <pthread.h>

class Condition;

// Wraps pthread_mutex_t for convenience
class Mutex {
public:
    Mutex() {
        int err = pthread_mutex_init(&m_mutex, nullptr);
        assert(!err);
    }

    ~Mutex() { pthread_mutex_destroy(&m_mutex); }

    bool Try() { return pthread_mutex_trylock(&m_mutex) == 0; }
    void Lock() { pthread_mutex_lock(&m_mutex); }
    void Unlock() { pthread_mutex_unlock(&m_mutex); }

private:
    friend Condition;
    pthread_mutex_t m_mutex;
};

// Sets mutex and automatically cleans up after itself
class ScopedMutex {
public:
    ScopedMutex(Mutex& lock)
        : m_lock(&lock)
    { m_lock->Lock(); }

    ~ScopedMutex() { m_lock->Unlock(); }

private:
    Mutex* m_lock;
};
