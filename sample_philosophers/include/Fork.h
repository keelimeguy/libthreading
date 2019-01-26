#pragma once

#include "Mutex.h"

// A fork is simply a (try) mutex
class Fork : private Mutex {
public:
    bool PickUp(int id) {
        if (Try()) {
            // Record who picked up the fork
            m_holder = id;
            return true;
        }
        return false;
    }

    void PutDown() {
        m_holder = -1;
        Unlock();
    }

    inline int Holder() { return m_holder; }

private:
    int m_holder = -1;
};

// Acts as a ScopedLock of fork, for convenience
class ChooseFork {
public:
    ChooseFork(Fork* fork, int id)
        : m_fork(fork), m_held(false), m_id(id)
    {
        // Record whether we picked up the fork
        if (m_fork) m_held = m_fork->PickUp(m_id);
    }

    ~ChooseFork() {
        // If we are holding the fork, then put it down before destructing
        if (m_held) m_fork->PutDown();
    }

                           // A bit redundant but that's fine
    inline bool IsHeld() { return m_held && m_fork->Holder() == m_id; }

private:
    Fork* m_fork;
    bool m_held;
    int m_id;
};
