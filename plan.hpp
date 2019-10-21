#ifndef PLAN_HPP
#define PLAN_HPP

#include <vector>
#include <map>
#include "star.hpp"
#include "block.hpp"
#include "def.hpp"
#include "statis.hpp"

class Plan {
 public:
  static Plan g_best;

 public:
  Plan() {}

  Plan(const Star star_matrix[WIDTH][LENGTH]);

  Plan(const Plan& p);

  void next_step(std::vector<Plan> &further_plans, 
                 Plan &best, Statis &stat) const;

  int score() const { return _score; }

  void print() const;

  bool operator <(const Plan &p) const;

 private:
  void pop(const Block &block2pop, Plan &result) const;

  void finish();

 private:
  Star _matrix[WIDTH][LENGTH];
  std::map<BID, const Block*> _block_map;
  std::vector<const Block*> _walk_path;
  int _score;
};

#endif // PLAN_HPP
