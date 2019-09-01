#include "block.hpp"

using namespace std;

short Block::max_id = 0;

void Block::print() const {
  cout << "id: " << _id << "\t";
  cout << "members: ";
  for (int i = 0; i < _members.size(); ++i) {
    cout << "(" << _members[i].x << "," << _members[i].y << ") " ;
  }
  cout << endl;
}

void add_neighbor_star_to_block(Star star_matrix[WIDTH][LENGTH], 
                                short x, 
                                short y, 
                                Block &block) {
  Star &center_star = star_matrix[x][y];
  if (INVALID_BLOCK_ID == center_star.block_id()) {
    block.add(Coord(x, y));
    center_star.set_block_id(block.id());

    if (y != 0) {
      Star &top_star = star_matrix[x][y - 1];
      if (center_star.type() == top_star.type()) {
        add_neighbor_star_to_block(star_matrix, x, y - 1, block);
      }
    }

    if (y != LENGTH - 1) {
      Star &bottom_star = star_matrix[x][y + 1];
      if (center_star.type() == bottom_star.type()) {
        add_neighbor_star_to_block(star_matrix, x, y + 1, block);
      }
    }

    if (x != 0) {
      Star &left_star = star_matrix[x - 1][y];
      if (center_star.type() == left_star.type()) {
        add_neighbor_star_to_block(star_matrix, x - 1, y, block);
      }
    }

    if (x != WIDTH - 1) {
      Star &right_star = star_matrix[x + 1][y];
      if (center_star.type() == right_star.type()) {
        add_neighbor_star_to_block(star_matrix, x + 1, y, block);
      }
    }
  }
}

void Block::init_blocks(Star star_matrix[WIDTH][LENGTH], 
                        vector<Block> &blocks) {
  for (short y = 0; y < LENGTH; ++y) {
    for (short x = 0; x < WIDTH; ++x) {
      if (INVALID_BLOCK_ID == star_matrix[x][y].block_id()) {
        Block new_block;
        new_block.set_id(Block::max_id++);
        add_neighbor_star_to_block(star_matrix, x, y, new_block);
        if (new_block._members.size() >= 2) {
          blocks.push_back(new_block);
        } else {
          for (int i = 0; i < new_block._members.size(); ++i) {
            star_matrix[new_block._members[i].x][new_block._members[i].y]
                .set_block_id(INVALID_BLOCK_ID);
          }
          --Block::max_id;
        }
      }
    }
  }
}
