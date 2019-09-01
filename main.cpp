#include <iostream>
#include <vector>
#include <assert.h>
#include <set>

#include "star.hpp"
#include "def.hpp"
#include "plan.hpp"

using namespace std;

int main(int argc, char **argv) {
  Star stars[STAR_COUNT];
  Star star_matrix[WIDTH][LENGTH];
  
  if (argc > 1) {
    if (Star::read_stars_from_file(argc, argv, stars)) {
      cerr << "Fail to read stars" << endl;
      return 1;
    }
  } else {
    Star::init_stars(stars);
  }
  Star::init_matrix(stars, star_matrix);
  Star::print_matrix(star_matrix);

  Plan root(star_matrix);
  root.print();
  Plan::best_plan = root;

  set<Plan> plans;
  cout << "init plans" << endl;
  root.next_step(plans);

  /*int round = 0;
  set<Plan> further_plans;
  while (!plans.empty()) {
    cout << "round: " << ++round << endl;
    set<Plan>::iterator it;
    further_plans.clear();
    for (it = plans.begin(); it != plans.end(); ++it) {
      it->next_step(further_plans);
    }
    plans = further_plans;
  }*/

  cout << endl << "BEST: " << endl;
  Plan::best_plan.print();

  return 0;
}
