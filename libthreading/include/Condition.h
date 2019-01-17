#pragma once

#include <cassert>
#include <pthread.h>

#include "Mutex.h"

// Wraps pthread_cond_t for convenience
class Condition {
public:
    Condition() {
        int err = pthread_cond_init(&m_cond, nullptr);
        assert(!err);
    }

    ~Condition() { pthread_cond_destroy(&m_cond); }

    void Wait(Mutex &mutex) { pthread_cond_wait(&m_cond, &(mutex.m_mutex)); }
    void Signal() { pthread_cond_signal(&m_cond); }
    void Broadcast() { pthread_cond_broadcast(&m_cond); }

private:
    pthread_cond_t m_cond;
};
