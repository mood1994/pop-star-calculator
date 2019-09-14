#ifndef UTIL_HPP
#define UTIL_HPP

#include <set>
#include <iostream>

#define DIVIDE_AND_CEIL(A, B) ((A) + (B) - 1) / (B)

template <class T, class R, int N>
class Hash_set {
 public:
  R insert (const T& val) {
    return _buckets[val.hash(N)].insert(val);
  }

  void clear() {
    for (int i = 0; i < N; ++i) {
      _buckets[i].clear();
    }
  }

  uint size() { 
    uint sz = 0;
    for (int i = 0; i < N; ++i) {
      sz += _buckets[i].size();
    }
    return sz;
  }

 private:
  std::set<T> _buckets[N];
};

#endif // UTIL_HPP
