#include "monitor.hpp"
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
