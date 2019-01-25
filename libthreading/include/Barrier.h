/*=============================================================================
    Copyright (c) 2019 Keelin Becker-Wheeler
    Barrier.h
    Distributed under the GNU GENERAL PUBLIC LICENSE
    See https://github.com/keelimeguy/libthreading
==============================================================================*/
#pragma once

#include <cassert>
#include <pthread.h>

// Wraps pthread_barrier_t for convenience
class Barrier {
public:
    Barrier(int count) {
        int err = pthread_barrier_init(&m_barrier, nullptr, count);
        assert(!err);
    }

    ~Barrier() { pthread_barrier_destroy(&m_barrier); }

    void Wait() { pthread_barrier_wait(&m_barrier); }

private:
    pthread_barrier_t m_barrier;
};
