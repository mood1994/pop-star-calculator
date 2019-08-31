#include <iostream>
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

void init_star_matrix(Star stars[STAR_COUNT], Star *star_matrix[WIDTH][LENGTH]) {
  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      star_matrix[x][y] = &stars[x + y * WIDTH];
    }
  }
}

void print_star_matrix(Star *star_matrix[WIDTH][LENGTH]) {
  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      cout << star_matrix[x][y]->type();
    }
    cout << endl;
  }
  cout << endl;
}

int main(int argc, char **argv) {
  Star stars[STAR_COUNT];
  Star *star_matrix[WIDTH][LENGTH];
  
  if (argc > 1) {
    if (Star::read_stars_from_file(argc, argv, stars)) {
      cerr << "Fail to read stars" << endl;
      return 1;
    }
  } else {
    Star::init_stars(stars);
  }
  init_star_matrix(stars, star_matrix);

  print_star_matrix(star_matrix);

  vector<Block> blocks;
  Block::init_blocks(star_matrix, blocks);
  for (int i = 0; i < blocks.size(); ++i) {
    blocks[i].print();
  }

  return 0;
}
