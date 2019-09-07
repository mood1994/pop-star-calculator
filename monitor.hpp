#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <time.h>
#include "def.hpp"

#define DEBUG_DO(code) \
if (mon.debug()) { code; }

class Monitor {
 public:
  Monitor();
  ~Monitor();

  void set_debug(bool arg_debug) { _debug = arg_debug; }

  bool debug() { return _debug; }

  llong total_plan_cnt() { return _total_plan_cnt; }

  void inc_total_plan(llong n = 1) { _total_plan_cnt += n; }

  llong discard_plan_cnt() { return _discard_plan_cnt; }
  
  void inc_discard_plan(llong n = 1) { _discard_plan_cnt += n; }

  llong finish_plan_cnt() { return _finish_plan_cnt; }

  void inc_finish_plan(llong n = 1) { _finish_plan_cnt += n; }

  llong quick_discard() { return _quick_discard; }

  void inc_quick_discard(llong n = 1) { _quick_discard += n; }

  void print();

  void reset() {
    _total_plan_cnt = 0;
    _discard_plan_cnt = 0;
    _quick_discard = 0;
    _finish_plan_cnt = 0;
  }

 private:
  time_t _start_time;
  time_t _stop_time;
  bool _debug;
  llong _total_plan_cnt;
  llong _discard_plan_cnt;
  llong _finish_plan_cnt;
  llong _quick_discard;
};

extern Monitor mon;

#endif // MONITOR_HPP
