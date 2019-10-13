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

using namespace std;

void get_options(int argc, char **argv, const char **file, bool &debug, 
                 int &thd_num) {
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--debug") == 0 ||
        strcmp(argv[i], "-d") == 0) {
      debug = true;
      continue;
    }
    if (strcmp(argv[i], "--file") == 0 ||
        strcmp(argv[i], "-f") == 0) {
      if (i + 1 < argc) {
        *file = argv[i + 1];
        i += 1;
      }
    }
    if (strcmp(argv[i], "--thread") == 0 ||
        strcmp(argv[i], "-t") == 0) {
      if (i + 1 < argc) {
        thd_num = atoi(argv[i + 1]);
        i += 1;
      }
    }
  }
}

int main(int argc, char **argv) {
  Star star_matrix[WIDTH][LENGTH];
  
  const char *file_path = NULL;
  bool debug = false;
  int thd_num = 1;
  get_options(argc, argv, &file_path, debug, thd_num);

  if (file_path) {
    cout << "Using file matrix" << endl;
    if (Star::read_matrix_from_file(file_path, star_matrix)) {
      cerr << "Fail to read stars" << endl;
      return 1;
    }
  } else {
    cout << "Using random matrix" << endl;
    Star::make_random_matrix(star_matrix);
  }
  Star::print_matrix(star_matrix);

  DEBUG_DO(cout << "Initializing root" << endl);
  Plan root(star_matrix);
  DEBUG_DO(root.print());
  Plan::g_best = root;
  Statis total;

  vector<Plan> first_plans;
  root.next_step(first_plans, Plan::g_best, total);
  Mini_matrix::g_hash_set.clear();
  total.reset();

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

    total.reset();
    for (it = units.begin(); it != units.end(); ++it) {
      Calc_unit *curr = *it;
      if (curr->best().score() > Plan::g_best.score()) {
        Plan::g_best = curr->best();
      }
      total = total + curr->statis();
    }
    Mini_matrix::g_hash_set.clear();

    for (it = units.begin(); it != units.end();) {
      if (!(*it)->has_next()) {
        delete *it;
        units.erase(it);
      } else {
        ++it;
      }
    }

    time_t curr_time = time(NULL);
    cout << "** ROUND: " << round++ << " **" << endl;
    total.print();
    cout << "cost: " << difftime(curr_time, start_time) << " (s)" << endl;
    cout << endl;
  }

  cout << endl << "BEST: " << endl;
  Plan::g_best.print();
  // TODO: replay the path

  return 0;
}
