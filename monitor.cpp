#include "monitor.hpp"
#include "block.hpp"
#include <iostream>

using namespace std;

Monitor mon;

Monitor::Monitor() {
  _debug = false;
  _total_plan_cnt = 0;
  _discard_plan_cnt = 0;
  _finish_plan_cnt = 0;
  _start_time = time(NULL);
}

Monitor::~Monitor() {
  _stop_time = time(NULL);
  cout << "Time cost: " << _stop_time - _start_time << " (s)" << endl;
}

void Monitor::print() {
  time_t now_time=time(NULL);
  tm*  t_tm = localtime(&now_time);
  cout << "total_plans:   " << _total_plan_cnt << endl
       << "discard_plans: " << _discard_plan_cnt << endl      
       << "quick_discard: " << _quick_discard << endl
       << "finish_plans:  " << _finish_plan_cnt << endl
       << "global blocks: " << Block::global_blocks.size() << endl
       << "at: " << asctime(t_tm) << endl;
}
