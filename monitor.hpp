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
  
  void inc_total_plan(int64 n = 1) { _total_plan_cnt += n; }

  void inc_discard_plan(int64 n = 1) { _discard_plan_cnt += n; }

  void inc_finish_plan(int64 n = 1) { _finish_plan_cnt += n; }

  int64 total_plan_cnt() { return _total_plan_cnt; }

  int64 discard_plan_cnt() { return _discard_plan_cnt; }

  int64 finish_plan_cnt() { return _finish_plan_cnt; }

  void reset() {
    _total_plan_cnt = 0;
    _discard_plan_cnt = 0;
    _finish_plan_cnt = 0;
  }

 private:
  time_t _start_time;
  time_t _stop_time;
  bool _debug;
  int64 _total_plan_cnt;
  int64 _discard_plan_cnt;
  int64 _finish_plan_cnt;
};

extern Monitor mon;

#endif // MONITOR_HPP
