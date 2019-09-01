#include "plan.hpp"
#include <iostream>

using namespace std;

int calc_pop_score(uint pop_count) {
  return pop_count * pop_count * 5;
}

int calc_reward_score(uint left_count) {
  static const uint MAX_REWARD_SCORE = 3000;
  return MAX_REWARD_SCORE - left_count * 5;
}

void reorg_neighbor(Star matrix[WIDTH][LENGTH],
                    const vector<Star *> &moved_stars, 
                    set<int> &out_blocks, 
                    vector<Block> &new_blocks) {
  /*for (int i = 0; i < moved_stars.size(); ++i) {
    int block_id = moved_stars[i]->block_id();
    if (block_id != INVALID_BLOCK_ID) {
      out_blocks.insert(block_id);
    }
  }

  for (int x = 0; x < WIDTH; ++x) {
    for (int y = 0; y < LENGTH; ++y) {
      Star *s = matrix[x][y];
      if (out_blocks.count(s->block_id()) > 0) {
        //s->set_block_id(INVALID_BLOCK_ID);
      }
    }
  }*/

  // TODO: assign the block id again.
}

Plan Plan::best_plan;

Plan::Plan(const Star star_matrix[WIDTH][LENGTH]) {
  cout << "Initializing plan" << endl;
  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < LENGTH; ++j) {
      _matrix[i][j] = star_matrix[i][j];
      _matrix[i][j].set_block_id(INVALID_BLOCK_ID);
    }
  }
  Block::init_blocks(_matrix, _blocks);
  int _score = 0;
}

void Plan::next_step(set<Plan> &further_plans) const {
  for (int i = 0; i < _blocks.size(); ++i) {
    Plan res;
    pop(i, res);
    res.print();
    if (!res._blocks.empty()) {
      if (further_plans.insert(res).second == false) {
        cout << "Plan was discarded for duplication." << endl;
      }
    } else {
      res.finish();
      if (res._score > best_plan._score) {
        best_plan = res;
        cout << "New best score: " << res._score << endl;
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
    cout << _walk_path[i] << " ";
  }
  cout << endl;

  cout << "BLOCKS: " << endl;
  for (int i = 0; i < _blocks.size(); ++i) {
    _blocks[i].print();
  }
  cout << "=================================================" << endl;
}

bool Plan::operator <(const Plan &p) const {
  if (_score < p._score) {
    return true;
  }
  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < LENGTH; ++j) {
      short id1 = _matrix[i][j].id();
      short id2 = p._matrix[i][j].id();
      if (id1 == id2) {
        continue;
      } else if (id1 < id2) {
        return true;
      } else if (id1 > id2) {
        return false;
      }
    }
  }
  return false;
}


void Plan::pop(int block2pop, Plan& result) const {
  // Update _walk_path
  result._walk_path = _walk_path;
  result._walk_path.push_back(block2pop);

  // Remove stars from _matrix
  memcpy(result._matrix, _matrix, sizeof(_matrix));
  const vector<Coord> &stars2pop = _blocks[block2pop].members();
  for (int i = 0; i < stars2pop.size(); ++i) {
    result._matrix[stars2pop[i].x][stars2pop[i].y] = Star::INVALID;
  }

  // Update _score
  result._score = _score + calc_pop_score(stars2pop.size());

  // Reorgnize _blocks
  vector<Star *> moved_stars;
  for (int x = 0; x < WIDTH; ++x) {
    // Move down
    for (int y = LENGTH - 1; y >= 0; --y) {
      // If one is NULL, exchange with first not NULL one ahead.
      if (result._matrix[x][y] == Star::INVALID) {
        bool all_null = true;
        for (int i = y; i >= 0; --i) {
          if (result._matrix[x][i] != Star::INVALID) {
            all_null = false; 
            result._matrix[x][y] = result._matrix[x][i];
            result._matrix[x][i] = Star::INVALID;
            moved_stars.push_back(&result._matrix[x][y]);
            break;
          }
        }
        if (all_null) {
          break;
        }
      }
    }
  }

  for (int x = 0; x < WIDTH; ++x) {
    // Move left
    // If the bottom one is NULL, all the column must be NULL.
    if (result._matrix[x][LENGTH - 1] != Star::INVALID)
      continue;
    for (int y = 0; y < LENGTH; ++y) {
      Star &s = result._matrix[x][y];
      if (s != Star::INVALID) {
        moved_stars.push_back(&s);
      }
    }
    for (int i = x + 1; i < WIDTH; ++i) {
      for (int y = 0; y < LENGTH; ++y) {
        if (result._matrix[i][y] != Star::INVALID) {
          result._matrix[i - 1][y] = result._matrix[i][y];
          result._matrix[i][y] = Star::INVALID;
        }
      }
    }
  }

  set<int> out_blocks;
  vector<Block> new_blocks;
  reorg_neighbor(result._matrix, moved_stars, out_blocks, new_blocks);

  out_blocks.insert(block2pop);
  for (int i = 0; i < _blocks.size(); ++i) {
    if (out_blocks.count(_blocks[i].id()) == 0) {
      result._blocks.push_back(_blocks[i]);
    }
  }

  for (int i = 0; i < new_blocks.size(); ++i) {
    result._blocks.push_back(new_blocks[i]);
  }

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
  cout << "Plan finish. Get reward " << reward << endl;
  _score += reward;
}
