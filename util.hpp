#ifndef UTIL_HPP
#define UTIL_HPP

#include <set>
#include <pthread.h>
#include <iostream>

#include "def.hpp"

#define DIVIDE_AND_CEIL(A, B) ((A) + (B) - 1) / (B)

#ifdef DEBUG
#  define DEBUG_DO(code) code;
#else
#  define DEBUG_DO(code) ;
#endif


template <class T, class R, int N>
class Hash_set {
 public:
  Hash_set() {
    for (int i = 0; i < N; ++i) {
      if (pthread_mutex_init(&_bucket_locks[i], NULL) != 0) {
        throw "Failed to init mutex.";
      }
    }
  }

  ~Hash_set() {
    for (int i = 0; i < N; ++i) {
      if (pthread_mutex_destroy(&_bucket_locks[i]) != 0) {
        throw "Failed to destroy mutex.";
      }
    }
  }

  R insert (const T& val) {
    uint hash = val.hash(N);
    pthread_mutex_lock(&_bucket_locks[hash]);
    R ret = _buckets[hash].insert(val);
    pthread_mutex_unlock(&_bucket_locks[hash]);
    return ret;
  }

  void clear() {
    for (int i = 0; i < N; ++i) {
      pthread_mutex_lock(&_bucket_locks[i]);
      _buckets[i].clear();
      pthread_mutex_unlock(&_bucket_locks[i]);
    }
  }

  uint size() {
    uint sz = 0;
    for (int i = 0; i < N; ++i) {
      sz += _buckets[i].size();
    }
    return sz;
  }

  void print_status() {
    std::cout << "bucket size detail: ";
    for (int i = 0; i < N; ++i) {
      std::cout << _buckets[i].size() << ' ';
    }
    std::cout << std::endl;
  }

 private:
  pthread_mutex_t _bucket_locks[N];
  std::set<T> _buckets[N];
};

#endif // UTIL_HPP
