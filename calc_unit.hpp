#ifndef CALC_UNIT_HPP
#define CALC_UNIT_HPP

#include <vector>
#include <pthread.h>
#include "plan.hpp"
#include "statis.hpp"

class Calc_unit {
 private:
  static void* thread_run(void *p);

 public:
  Calc_unit(const std::vector<Plan> &root_plans);

  void start_round();

  void join_round();

  bool has_next() const { return !_curr_plans->empty(); }

  const Plan& best() const { return _best_plan; }

  llong tid() const { return (llong) _tid; }

  Statis statis() const { return _statis; }

 private:
  pthread_t _tid;
  Plan _best_plan;
  Statis _statis;
  std::vector<Plan> _plans1;
  std::vector<Plan> _plans2;
  std::vector<Plan> *_curr_plans;
  std::vector<Plan> *_further_plans;
};

#endif // CALC_UNIT_HPP
