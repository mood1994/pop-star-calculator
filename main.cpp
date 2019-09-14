#include <iostream>
#include <vector>
#include <set>

#include "star.hpp"
#include "def.hpp"
#include "plan.hpp"
#include "monitor.hpp"
using namespace std;

void get_options(int argc, char **argv, const char **file, bool &debug) {
  *file = NULL; debug = false; for (int i = 1; i < argc; ++i) {
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
  }
}

int main(int argc, char **argv) {
  Star star_matrix[WIDTH][LENGTH];
  
  const char *file_path = NULL;
  bool debug = false;
  get_options(argc, argv, &file_path, debug);

  mon.set_debug(debug);

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
  Plan::best_plan = root;

  vector<Plan> plan_container_1;
  vector<Plan> plan_container_2;
  vector<Plan> *curr_plans = &plan_container_1;
  vector<Plan> *further_plans = &plan_container_2;
  vector<Plan> *tmp_ptr = NULL;
  Mtrx_hash_set mtrx_set;

  int round = 0;
  root.next_step(*curr_plans, mtrx_set);
  mtrx_set.clear();
  while (!curr_plans->empty()) {
    mon.reset();
    for (int i = 0; i < curr_plans->size(); ++i) {
      (*curr_plans)[i].next_step(*further_plans, mtrx_set);
    }
    curr_plans->clear();
    mtrx_set.clear();

    tmp_ptr = curr_plans;
    curr_plans = further_plans;
    further_plans = tmp_ptr;

    cout << "** ROUND: " << ++round << " **" << endl;
    mon.print();
  }

  cout << endl << "BEST: " << endl;
  Plan::best_plan.print();

  return 0;
}
