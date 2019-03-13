#include "block.hpp"

using namespace std;

void block::print() {
  cout << "id: " << _id << endl;
  cout << "members: " << endl;
  for (int i = 0; i < _members.size(); ++i) {
    _members[i]->print();
  }
  cout << endl;
}

void block::disband() {
  for (int i = 0; i < _members.size(); ++i) {
    _members[i]->set_block_id(INVALID_BLOCK_ID);
  }
  _members.clear();
}

void add_neighbor_star_to_block(star* star_matrix[WIDTH][LENGTH], int x, int y, block *p_block) {
  star *p_center_star = star_matrix[x][y];
  if (INVALID_BLOCK_ID == p_center_star->block_id()) {
    p_block->add(p_center_star);
    p_center_star->set_block_id(p_block->id());

    if (y != 0) {
      star *p_top_star = star_matrix[x][y - 1];
      if (p_center_star->type() == p_top_star->type()) {
        add_neighbor_star_to_block(star_matrix, x, y - 1, p_block);
      }
    }

    if (y != LENGTH - 1) {
      star *p_bottom_star = star_matrix[x][y + 1];
      if (p_center_star->type() == p_bottom_star->type()) {
        add_neighbor_star_to_block(star_matrix, x, y + 1, p_block);
      }
    }

    if (x != 0) {
      star *p_left_star = star_matrix[x - 1][y];
      if (p_center_star->type() == p_left_star->type()) {
        add_neighbor_star_to_block(star_matrix, x - 1, y, p_block);
      }
    }

    if (x != WIDTH - 1) {
      star *p_right_star = star_matrix[x + 1][y];
      if (p_center_star->type() == p_right_star->type()) {
        add_neighbor_star_to_block(star_matrix, x + 1, y, p_block);
      }
    }
  }
}

void init_blocks(star* star_matrix[WIDTH][LENGTH], vector<block> &blocks) {
  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      if (INVALID_BLOCK_ID == star_matrix[x][y]->block_id()) {
        block new_block;
        new_block.set_id(blocks.size());
        add_neighbor_star_to_block(star_matrix, x, y, &new_block);
        if (new_block.member_count() >= 2) {
          blocks.push_back(new_block);
        } else {
          new_block.disband();          
        }
      }
    }
  }
}
