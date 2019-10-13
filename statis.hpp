#ifndef STATIS_HPP
#define STATIS_HPP

#include "def.hpp"
#include <iostream>

class Statis {
 public:
  llong total_plan_cnt;
  llong discard_plan_cnt;
  llong finish_plan_cnt;
  llong quick_discard;

  Statis(): total_plan_cnt(0),
            discard_plan_cnt(0),
            finish_plan_cnt(0),
            quick_discard(0) {}

  Statis operator +(const Statis &s) const {
    Statis res = *this;
    res.total_plan_cnt += s.total_plan_cnt;
    res.discard_plan_cnt += s.discard_plan_cnt;
    res.finish_plan_cnt += s.finish_plan_cnt;
    res.quick_discard += s.quick_discard;
    return res;
  }

  void print() const {
    std::cout << "total_plans:   " << total_plan_cnt << std::endl
        << "discard_plans: " << discard_plan_cnt << std::endl
        << "quick_discard: " << quick_discard << std::endl
        << "finish_plans:  " << finish_plan_cnt << std::endl;
  }

  void reset() {
    total_plan_cnt = 0;
    discard_plan_cnt = 0;
    quick_discard = 0;
    finish_plan_cnt = 0;
  }

};

#endif // STATIS_HPP
