#pragma once

#include <cassert>
#include <pthread.h>

// Wraps pthread_rwlock_t for convenience
class RWLock {
public:
    RWLock() {
        int err = pthread_rwlock_init(&m_rwlock, nullptr);
        assert(!err);
    }

    ~RWLock() { pthread_rwlock_destroy(&m_rwlock); }

    bool TryRead() { return pthread_rwlock_tryrdlock(&m_rwlock) == 0; }
    bool TryWrite() { return pthread_rwlock_trywrlock(&m_rwlock) == 0; }
    void ReadLock() { pthread_rwlock_rdlock(&m_rwlock); }
    void WriteLock() { pthread_rwlock_wrlock(&m_rwlock); }
    void Unlock() { pthread_rwlock_unlock(&m_rwlock); }

private:
    pthread_rwlock_t m_rwlock;
};

// Sets rwlock for read and automatically cleans up after itself
class ScopedReadLock {
public:
    ScopedReadLock(RWLock& lock)
        : m_lock(&lock)
    { m_lock->ReadLock(); }

    ~ScopedReadLock() { m_lock->Unlock(); }

private:
    RWLock* m_lock;
};

// Sets rwlock for write and automatically cleans up after itself
class ScopedWriteLock {
public:
    ScopedWriteLock(RWLock& lock)
        : m_lock(&lock)
    { m_lock->WriteLock(); }

    ~ScopedWriteLock() { m_lock->Unlock(); }

private:
    RWLock* m_lock;
};
