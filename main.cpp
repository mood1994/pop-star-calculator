#include <iostream>
#include <vector>
#include <set>
#include <time.h>

#include "star.hpp"
#include "def.hpp"
#include "plan.hpp"
#include "monitor.hpp"
using namespace std;

void get_options(int argc, char **argv, const char **file, bool &debug) {
  *file = NULL;
  debug = false;
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
  }
}

int main(int argc, char **argv) {
  Star stars[STAR_COUNT];
  Star star_matrix[WIDTH][LENGTH];
  
  const char *file_path = NULL;
  bool debug = false;
  get_options(argc, argv, &file_path, debug);

  mon.set_debug(debug);

  if (file_path) {
    if (Star::read_stars_from_file(file_path, stars)) {
      cerr << "Fail to read stars" << endl;
      return 1;
    }
  } else {
    Star::init_stars(stars);
  }
  Star::init_matrix(stars, star_matrix);

  DEBUG_DO(cout << "Initializing root" << endl);
  Plan root(star_matrix);
  DEBUG_DO(root.print());
  Plan::best_plan = root;

  set<Plan> plans;
  root.next_step(plans);

  int round = 0;
  set<Plan> further_plans;
  while (!plans.empty()) {
    mon.reset();
    set<Plan>::iterator it;
    further_plans.clear();
    for (it = plans.begin(); it != plans.end(); ++it) {
      it->next_step(further_plans);
    }
    time_t now_time=time(NULL);
    tm*  t_tm = localtime(&now_time);
    cout << "round: " << ++round << endl
         << "total_plans:   " << mon.total_plan_cnt() << endl
         << "discard_plans: " << mon.discard_plan_cnt() << endl      
         << "finish_plans:  " << mon.finish_plan_cnt() << endl
         << "at: " << asctime(t_tm) << endl;
    plans = further_plans;
  }

  cout << endl << "BEST: " << endl;
  Plan::best_plan.print();

  return 0;
}
