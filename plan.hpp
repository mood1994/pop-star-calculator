#ifndef PLAN_HPP
#define PLAN_HPP

#include <vector>
#include <map>
#include "star.hpp"
#include "block.hpp"
#include "def.hpp"
#include "statis.hpp"

class Track_replayer;

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

  const std::vector<const Block *>& track() const { return _track; }

  void print() const;

  bool operator <(const Plan &p) const;

 private:
  void pop(const Block &block2pop, Plan &result) const;

  void finish();

 private:
  Star _matrix[WIDTH][LENGTH];
  std::map<BID, const Block*> _block_map;
  std::vector<const Block*> _track;
  int _score;

  friend class Track_replayer;
};

class Track_replayer {
 public:
  static Plan g_root_plan;

 public:
  Track_replayer() {}

  ~Track_replayer();

  const Plan *replay(const std::vector<BID> &track);

 private:
  std::vector<BID> _last_track;
  std::vector<Plan *> _last_result;
};

#endif // PLAN_HPP
