#include "plan.hpp"
#include <iostream>
#include "monitor.hpp"

using namespace std;

int calc_pop_score(uint pop_count) {
  return pop_count * pop_count * 5;
}

int calc_reward_score(uint left_count) {
  static const uint MAX_REWARD_SCORE = 3000;
  return MAX_REWARD_SCORE - left_count * 5;
}

bool is_block_independent(const Block &a, const Block &b) {
  int a_min_x = 0;
  int a_max_x = 0;
  a.get_bound(a_min_x, a_max_x);

  int b_min_x = 0;
  int b_max_x = 0;
  b.get_bound(b_min_x, b_max_x);
  
  return (b_min_x - a_max_x > 1 || a_min_x - b_max_x > 1);
}

void move_down_star_and_disband_block(Star matrix[WIDTH][LENGTH], 
                                      int x,
                                      int up_y,
                                      int down_y,
                                      const map<int, const Block*> &block_map,
                                      set<int> &out_blocks) {
  // Note: down_star always be INVALID.
  Star &up_star = matrix[x][up_y];
  Star &down_star = matrix[x][down_y];
  // 1. disband old block
  int disband_id = up_star.block_id();
  map<int, const Block*>::const_iterator it;
  if (disband_id != INVALID_BLOCK_ID) {
    out_blocks.insert(disband_id);
    it = block_map.find(disband_id);
    it->second->disband(matrix);
  }
  // 2. move up_star down
  down_star = up_star;
  up_star = Star::INVALID;
  // 3. disband new neighbor block to merge
  int my_type = down_star.type();
  // Note: top must be INVALID. No need to check.
  if (x > 0) {
    Star &left = matrix[x - 1][down_y]; 
    if (left.type() == my_type && 
        left.block_id() != INVALID_BLOCK_ID) {
      out_blocks.insert(left.block_id());
      it = block_map.find(left.block_id());
      it->second->disband(matrix);
    }
  }
  if (x < WIDTH - 1) {
    Star &right = matrix[x + 1][down_y];
    if (right.type() == my_type &&
        right.block_id() != INVALID_BLOCK_ID) {
      out_blocks.insert(right.block_id());
      it = block_map.find(right.block_id());
      it->second->disband(matrix);
    }
  }
  if (down_y < LENGTH - 1) {
    Star &down = matrix[x][down_y + 1];
    if (down.type() == my_type &&
        down.block_id() != INVALID_BLOCK_ID) {
      out_blocks.insert(down.block_id());
      it = block_map.find(down.block_id());
      it->second->disband(matrix);
    }
  }
}

void remove_column_and_disband_block(Star matrix[WIDTH][LENGTH], 
                                     int x, 
                                     const map<int, const Block*> &block_map,
                                     set<int> &out_blocks) {
  // disband blocks of neighbor columns.
  map<int, const Block*>::const_iterator it;
  if (x > 0) {
    for (int y = 0; y < LENGTH; ++y) {
      int left_id = matrix[x - 1][y].block_id();
      if (left_id != INVALID_BLOCK_ID) {
        it = block_map.find(left_id);
        it->second->disband(matrix);
        out_blocks.insert(left_id);
      }
    }
  }
  // move all right columns left.
  for (int i = x + 1; i < WIDTH; ++i) {
    for (int y = 0; y < LENGTH; ++y) {
      int right_id = matrix[i][y].block_id();
      if (right_id != INVALID_BLOCK_ID) {
        it = block_map.find(right_id);
        it->second->disband(matrix);
        out_blocks.insert(right_id);
      }
      matrix[i - 1][y] = matrix[i][y];
    }
  }
  if (matrix[WIDTH - 1][LENGTH - 1] != Star::INVALID) {
    for (int y = 0; y < LENGTH; ++y) {
      matrix[WIDTH - 1][y] = Star::INVALID;
    }
  }
}

Plan Plan::best_plan;

Plan::Plan(const Star star_matrix[WIDTH][LENGTH]) {
  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < LENGTH; ++j) {
      _matrix[i][j] = star_matrix[i][j];
      _matrix[i][j].set_block_id(INVALID_BLOCK_ID);
    }
  }
  Block::init_blocks(_matrix, _block_map);
  int _score = 0;
}

void Plan::next_step(set<Plan> &further_plans) const {
  map<int, const Block*>::const_iterator it;
  for (it = _block_map.begin(); it != _block_map.end(); ++it) {
    mon.inc_total_plan();
    if (!_walk_path.empty()) {
      const Block& last_block = *_walk_path[_walk_path.size() - 1];
      if (is_block_independent(last_block, *(it->second)) && 
          it->second->id() < last_block.id()) {
        mon.inc_discard_plan();
        mon.inc_quick_discard();
        continue;
      }
    }
    Plan res;
    pop(*(it->second), res);
    DEBUG_DO(res.print());
    if (!res._block_map.empty()) {
      if (further_plans.insert(res).second == false) {
        mon.inc_discard_plan();
        DEBUG_DO(cout << "Plan was discarded for duplication." << endl);
      }
    } else {
      mon.inc_finish_plan();
      res.finish();
      if (res._score > best_plan._score) {
        best_plan = res;
        DEBUG_DO(cout << "New best score: " << res._score << endl);
      }
    }
  }
}

void Plan::print() const {
  cout << "=================================================" << endl;
  cout << "MATRIX: " << endl;
  Star::print_matrix(_matrix);

  cout << "SCORE: " << _score << endl;
  cout << "WALK_PATH: ";
  for (int i = 0; i < _walk_path.size(); ++i) {
    cout << _walk_path[i]->id() << " ";
  }
  cout << endl;

  cout << "BLOCKS: " << endl;
  map<int, const Block*>::const_iterator it;
  for (it = _block_map.begin(); it != _block_map.end(); ++it) {
    it->second->print();
  }
  cout << "=================================================" << endl;
}

bool Plan::operator <(const Plan &p) const {
  if (_score < p._score) {
    return true;
  }

  if (_block_map.size() < p._block_map.size()) {
    return true;
  } else if (_block_map.size() > p._block_map.size()) {
    return false;
  }

  map<int, const Block*>::const_iterator it;
  map<int, const Block*>::const_iterator it2;
  for (it = _block_map.begin(), it2 = p._block_map.begin(); 
       it != _block_map.end(); 
       ++it, ++it2) {
    int id = it->second->id();
    int id2 = it2->second->id();
    if (id == id2) {
      continue;
    } else if (id < id2) {
      return true;
    } else if (id > id2) {
      return false;
    }
  }
  return false;
}

void Plan::pop(const Block &block2pop, Plan& result) const {
  // Update _walk_path
  result._walk_path = _walk_path;
  result._walk_path.push_back(&block2pop);

  // Remove stars from _matrix
  memcpy(result._matrix, _matrix, sizeof(_matrix));
  const vector<Coord> &stars2pop = block2pop.members();
  for (int i = 0; i < stars2pop.size(); ++i) {
    result._matrix[stars2pop[i].x][stars2pop[i].y] = Star::INVALID;
  }

  // Update _score
  result._score = _score + calc_pop_score(stars2pop.size());

  // Reorgnize _matrix and _block
  // Move down
  set<int> out_blocks;
  for (int x = 0; x < WIDTH; ++x) {
    for (int y = LENGTH - 1; y >= 0; --y) {
      // If one is NULL, exchange with first not NULL one ahead.
      if (result._matrix[x][y] == Star::INVALID) {
        bool all_null = true;
        for (int up = y - 1; up >= 0; --up) {
          if (result._matrix[x][up] != Star::INVALID) {
            all_null = false; 
            move_down_star_and_disband_block(result._matrix,
                                             x, up, y,
                                             _block_map,
                                             out_blocks);
            break;
          }
        }
        if (all_null) {
          break;
        }
      }
    }
  }

  // Move left
  int x = 0;
  while (x < WIDTH - 1) {
    // If the bottom one is NULL, all the column must be NULL.
    if (result._matrix[x][LENGTH - 1] != Star::INVALID) {
      x += 1;
      continue;
    }
    bool is_tail = true;
    for (int i = x + 1; i < WIDTH; ++i) {
      if (result._matrix[i][LENGTH - 1] != Star::INVALID) {
        is_tail = false;
      }
    }
    if (is_tail) {
      break;
    }
    remove_column_and_disband_block(result._matrix, x, 
                                    _block_map, out_blocks);
  }

  out_blocks.insert(block2pop.id());
  map<int, const Block*>::const_iterator it;
  for (it = _block_map.begin(); it != _block_map.end(); ++it) {
    if (out_blocks.count(it->second->id()) == 0) {
      pair<int, const Block*> val(it->second->id(), it->second);
      result._block_map.insert(val);
    }
  }

  Block::init_blocks(result._matrix, result._block_map);
}

void Plan::finish() {
  uint left_stars = 0;
  for (int x = 0; x < WIDTH; ++x) {
    for (int y = 0; y < LENGTH; ++y) {
      if (_matrix[x][y] != Star::INVALID) {
        left_stars += 1;
      }
    }
  }
  int reward = calc_reward_score(left_stars);
  DEBUG_DO(cout << "Plan finish. Get reward " << reward << endl);
  _score += reward;
}
