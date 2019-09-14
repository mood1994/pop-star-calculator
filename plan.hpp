#ifndef PLAN_HPP
#define PLAN_HPP

#include <vector>
#include <set>
#include <map>
#include "star.hpp"
#include "block.hpp"
#include "def.hpp"

class Plan {
 public:
  static Plan best_plan;

 public:
  Plan(const Star star_matrix[WIDTH][LENGTH]);

  Plan(const Plan& p);

  void next_step(std::vector<Plan> &further_plans, Mtrx_hash_set &mtrx_set) const;

  void print() const;

  bool operator <(const Plan &p) const;

 private:
  Plan() {}

  void pop(const Block &block2pop, Plan &result) const;

  void finish();

 private:
  Star _matrix[WIDTH][LENGTH];
  std::map<int, const Block*> _block_map;
  std::vector<const Block*> _walk_path;
  int _score;
};

#endif // PLAN_HPP
