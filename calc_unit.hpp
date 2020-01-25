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

  bool has_next() const { return _old_tracks->size() > 1;}

  const Plan& best() const { return _best_plan; }

  pthread_t tid() const { return _tid; }

  Statis statis() const { return _statis; }

 private:
  pthread_t _tid;
  Plan _best_plan;
  Statis _statis;
  std::vector<BID> _tracks1;
  std::vector<BID> _tracks2;
  std::vector<BID> *_old_tracks;
  std::vector<BID> *_new_tracks;
  Track_replayer _replayer;
};

#endif // CALC_UNIT_HPP
