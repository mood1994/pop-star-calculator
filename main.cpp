#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <assert.h>

#include "star.hpp"
#include "def.hpp"
#include "block.hpp"

using namespace std;

// global function
int calc_kill_score(uint kill_count) {
  return kill_count * kill_count * 5;
}

int calc_reward_score(uint left_count) {
  static const uint MAX_REWARD_SCORE = 3000;
  return MAX_REWARD_SCORE - left_count * 5;
}

int main(int argc, char **argv) {
  star stars[STAR_COUNT];
  srand((unsigned)time(NULL));
  for (int i = 0; i < STAR_COUNT; ++i) {
    int rand_type = rand() % STAR_TYPE_COUNT;
    stars[i].set_type(rand_type);
    stars[i].set_id(i);
  }

  star* star_matrix[WIDTH][LENGTH];
  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      star_matrix[x][y] = &stars[x + y * WIDTH];
    }
  }

  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      cout << star_matrix[x][y]->type();
    }
    cout << endl;
  }
  cout << endl;

  vector<block> blocks;
  init_blocks(star_matrix, blocks);
  for (int i = 0; i < blocks.size(); ++i) {
    blocks[i].print();
  }
  
  return 0;
}
