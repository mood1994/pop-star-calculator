#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <list>

#include "star.hpp"
#include "def.hpp"
#include "plan.hpp"
#include "statis.hpp" 
#include "calc_unit.hpp"
#include "conf.hpp"

using namespace std;

Config g_conf;

int main(int argc, char **argv) {
  Star star_matrix[WIDTH][LENGTH];
  
  g_conf.parse_conf(argc, argv);

  if (g_conf.file) {
    if (Star::read_matrix_from_file(g_conf.file, star_matrix)) {
      cerr << "Fail to read stars" << endl; return 1;
    }
  } else {
    Star::make_random_matrix(star_matrix);
  }
  Star::print_matrix(star_matrix);

  DEBUG_DO(cout << "Initializing root" << endl);
  thd_id_mgr = new BlockIDMgr();
  Plan root(star_matrix);
  DEBUG_DO(root.print());
  Plan::g_best = root;
  Track_replayer::g_root_plan = root;
  Statis total;

  vector<Plan> first_plans;
  root.next_step(first_plans, Plan::g_best, total);
  Mini_matrix::g_hash_set.clear();
  total.reset();
  delete thd_id_mgr;

  int thd_num = g_conf.thd_num;
  list<Calc_unit *> units;
  int plans_per_thd = first_plans.size() / thd_num;
  for (int i = 0; i < thd_num; ++i) {
    int start = i * plans_per_thd;
    int end = start + plans_per_thd;
    if (i == thd_num - 1) {
      end = first_plans.size();
    }

    vector<Plan> thd_plans;
    for (int j = start; j < end; ++j) {
      thd_plans.push_back(first_plans[j]);
    }

    Calc_unit *unit = new Calc_unit(thd_plans);
    units.push_back(unit);
  }

  list<Calc_unit *>::iterator it;
  uint round = 1;
  time_t start_time = time(NULL);
  while (units.size() > 0) {
    for (it = units.begin(); it != units.end(); ++it) {
      (*it)->start_round();
    }
    for (it = units.begin(); it != units.end(); ++it) {
      (*it)->join_round();
    }

    for (it = units.begin(); it != units.end(); ++it) {
      Calc_unit *curr = *it;
      if (curr->best().score() > Plan::g_best.score()) {
        Plan::g_best = curr->best();
      }
      // TODO: it's unbalance
      // cout << "TID: " << curr->tid() << endl;
      // curr->statis().print();
      // cout << endl;
      total = total + curr->statis();
    }

    time_t curr_time = time(NULL);
    cout << "** ROUND: " << round++ << " **" << endl;
    total.print();
    cout << "cost: " << difftime(curr_time, start_time) << " (s)" << endl;
    cout << "Mini_matrix.size: " << Mini_matrix::g_hash_set.size() << endl;
    cout << "Block matrix size: " << Block::g_hash_set.size() << endl;
    cout << endl;

    for (it = units.begin(); it != units.end();) {
      if (!(*it)->has_next()) {
        delete *it;
        units.erase(it);
      } else {
        ++it;
      }
    }
    total.reset();
    Mini_matrix::g_hash_set.clear();
  }

  cout << endl << "BEST: " << endl;
  Plan::g_best.print();
  // TODO: replay the path

  return 0;
}
