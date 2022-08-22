/*
 * @Author: Ryu-59073
 * @Date: 2022-07-25 01:05:58
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-25 01:46:58
 * @FilePath: /Cpp_code/async_prog/spinlock.h
 * @Description: 各式各样的自旋锁（posix spin loc, tas, cas)
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#ifndef ASYNC_SPINLOCK_H
#define ASYNC_SPINLOCK_H

#include <cstdint>
#include <atomic>

#include <pthread.h>
/**
 * @brief 简单的RAII锁封装
 * 
 * @tparam T 锁类型
 */
template<typename T>
class ScopedLock {
public:
    ScopedLock(T& mutex): m_mutex(mutex){
        m_mutex.lock();
        m_locked = true;
    }

    ~ScopedLock() {
        if(m_locked) {
            unlock();
        }
    }

    void lock() {
        if(!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock() {
        if(m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

    bool tryLock() {
        if(!m_locked) {
            m_locked = m_mutex.tryLock();
        }
        return m_locked;
    }
    
private:
    T& m_mutex;
    bool m_locked;
};

class NonCopyable{
public:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class SpinLock_POSIX: NonCopyable{
public:
    using Lock = ScopedLock<SpinLock_POSIX>;
    SpinLock_POSIX() {
        pthread_spin_init(&m_mutex, 0);
    }
    ~SpinLock_POSIX() {
        pthread_spin_destroy(&m_mutex);
    }
    void lock() {
        pthread_spin_lock(&m_mutex);
    }
    bool tryLock() {
        return !pthread_spin_trylock(&m_mutex);
    }
    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }

private:
    pthread_spinlock_t m_mutex;
};

/**
 * @brief std::atomic_flag TAS实现自旋锁
 * 
 */
class SpinLock_TAS: NonCopyable{
public:
    using Lock = ScopedLock<SpinLock_TAS>;
    SpinLock_TAS(): m_mutex(ATOMIC_FLAG_INIT) {
    }
    void lock() {
        while(m_mutex.test_and_set(std::memory_order_acquire)){
            // loop
        }
    }
    void unlock() {
        m_mutex.clear(std::memory_order_release);
    }
    bool tryLock() {
        return !m_mutex.test_and_set(std::memory_order_acquire);
    }
private:
    std::atomic_flag m_mutex;
};

/**
 * @brief std::atomic<bool> CAS实现自旋锁
 * 
 */
class SpinLock_CAS: NonCopyable{
public:
    using Lock = ScopedLock<SpinLock_CAS>;
    SpinLock_CAS(): m_mutex(false) {
    }
    void lock() {
        bool excepted = false;
        while(m_mutex.compare_exchange_strong(excepted, true) == false) {
            excepted = false;
        } 
    }
    void unlock() {
        m_mutex.store(false);
    }
    bool tryLock() {
        bool excepted = false;
        return m_mutex.compare_exchange_strong(excepted, true);
    }
private:
    std::atomic<bool> m_mutex;
};


#endif