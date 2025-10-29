// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (giantchen at gmail dot com)

#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include "Thread.h"

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace muduo
{

class MutexLock : boost::noncopyable  // C++ linux 端封装 mutex
{
 public:
  MutexLock()
    : holder_(0)
  {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock()
  {
    assert(holder_ == 0);
    pthread_mutex_destroy(&mutex_);
  }

  bool isLockedByThisThread()
  {
    return holder_ == CurrentThread::tid();
  }

  void assertLocked()
  {
    assert(isLockedByThisThread());
  }

  // internal usage

  void lock()
  {
    pthread_mutex_lock(&mutex_);
    holder_ = CurrentThread::tid();
  }

  void unlock()
  {
    holder_ = 0;
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t* getPthreadMutex() /* non-const */
  {
    return &mutex_;
  }

 private:

  pthread_mutex_t mutex_;
  pid_t holder_;
};

class MutexLockGuard : boost::noncopyable   //RAII 技术
{
 public:
  explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLockGuard()
  {
    mutex_.unlock();
  }

 private:

  MutexLock& mutex_;
};

}

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

//#define MutexLockGuard(x) static_assert(false, "Missing guard object name") //书中用得断言方式

//这样写成宏，只要这样写就会报错，必须 MutexLockGuard  lock(mutex); 定义，不能匿名定义

#endif  // MUDUO_BASE_MUTEX_H
