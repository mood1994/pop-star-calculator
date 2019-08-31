#ifndef PLAN_HPP
#define PLAN_HPP

#include <vector>
#include <set>
#include "star.hpp"
#include "block.hpp"
#include "def.hpp"

class Plan {
 public:
  static Plan best_plan;

 public:
  Plan(Star *star_matrix[WIDTH][LENGTH]);

  void next_step(std::set<Plan> &further_plans) const;

  void print() const;

  bool operator <(const Plan &p) const;

 private:
  Plan() {}
  void pop(int block2pop, Plan &result) const;
  void finish();

 private:
  Star *_matrix[WIDTH][LENGTH];
  std::vector<Block> _blocks;
  std::vector<int> _walk_path;
  int _score;
};

#endif // PLAN_HPP
