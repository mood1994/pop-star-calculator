#include "calc_unit.hpp"

using namespace std;

Calc_unit::Calc_unit(const std::vector<Plan> &root_plans) {
  _plans1 = root_plans;
  _curr_plans = &_plans1;
  _further_plans = &_plans2;
}

void* Calc_unit::thread_run(void *p) {
  Calc_unit &unit = *((Calc_unit *) p);
  vector<Plan> *tmp_ptr = NULL;

  unit._statis.reset();
  for (int i = 0; i < unit._curr_plans->size(); ++i) {
    (*unit._curr_plans)[i].next_step(*unit._further_plans, 
                                     unit._best_plan, 
                                     unit._statis);
  }

  unit._curr_plans->clear();
  tmp_ptr = unit._curr_plans;
  unit._curr_plans = unit._further_plans;
  unit._further_plans = tmp_ptr;

  return NULL;
}

void Calc_unit::start_round() {
  if (pthread_create(&_tid, NULL, Calc_unit::thread_run, this) != 0) {
    throw "Failed to create thread";
  }
}

void Calc_unit::join_round() {
  if (pthread_join(_tid, NULL) != 0) {
    throw "Failed to join thread";
  }
}
