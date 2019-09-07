#include "block.hpp"

using namespace std;

set<Block> Block::global_blocks;

void Block::print() const {
  cout << "id: " << _id << "\t";
  cout << "type: " << _type << "\t";
  cout << "members: ";
  for (int i = 0; i < _members.size(); ++i) {
    cout << "(" << _members[i].x << "," << _members[i].y << ") " ;
  }
  cout << endl;
}

bool Block::operator <(const Block &b) const {
  if (_members.size() < b._members.size()) {
    return true;
  } else if (_members.size() > b._members.size()) {
    return false;
  }

  for (int i = 0; i < _members.size(); ++i) {
    if (_members[i] == b._members[i]) {
      continue;
    } else if (_members[i] < b._members[i]) {
      return true;
    } else if (_members[i] > b._members[i]) {
      return false;
    }
  }

  return _type < b._type;
}

void Block::disband(Star matrix[WIDTH][LENGTH]) const {
  for (int i = 0; i < _members.size(); ++i) {
    matrix[_members[i].x][_members[i].y]
        .set_block_id(INVALID_BLOCK_ID);
  }
}

void add_neighbor_star_to_block(Star matrix[WIDTH][LENGTH], 
                                short x, 
                                short y, 
                                Block &block) {
  Star &center_star = matrix[x][y];
  if (INVALID_BLOCK_ID == center_star.block_id()) {
    block.add(Coord(x, y));
    center_star.set_block_id(block.id());

    if (y != 0) {
      Star &top_star = matrix[x][y - 1];
      if (center_star.type() == top_star.type()) {
        add_neighbor_star_to_block(matrix, x, y - 1, block);
      }
    }

    if (y != LENGTH - 1) {
      Star &bottom_star = matrix[x][y + 1];
      if (center_star.type() == bottom_star.type()) {
        add_neighbor_star_to_block(matrix, x, y + 1, block);
      }
    }

    if (x != 0) {
      Star &left_star = matrix[x - 1][y];
      if (center_star.type() == left_star.type()) {
        add_neighbor_star_to_block(matrix, x - 1, y, block);
      }
    }

    if (x != WIDTH - 1) {
      Star &right_star = matrix[x + 1][y];
      if (center_star.type() == right_star.type()) {
        add_neighbor_star_to_block(matrix, x + 1, y, block);
      }
    }
  }
}

void update_block_id(const Block &block, Star matrix[WIDTH][LENGTH]) {
  const vector<Coord> &members = block.members();
  for (int i = 0; i < members.size(); ++i) {
    Coord c = members[i];
    matrix[c.x][c.y].set_block_id(block.id());
  }
}

void Block::init_blocks(Star matrix[WIDTH][LENGTH], 
                        map<int, const Block*> &blocks) {
  pair<set<Block>::iterator, bool> ret;
  for (short y = 0; y < LENGTH; ++y) {
    for (short x = 0; x < WIDTH; ++x) {
      if (matrix[x][y] != Star::INVALID &&
          INVALID_BLOCK_ID == matrix[x][y].block_id()) {
        Block new_block;
        new_block.set_type(matrix[x][y].type());
        new_block.set_id(Block::global_blocks.size());
        add_neighbor_star_to_block(matrix, x, y, new_block);
        if (new_block._members.size() >= 2) {
          ret = Block::global_blocks.insert(new_block);
          const Block *p = &(*ret.first);
          if (!ret.second) {
            update_block_id(*p, matrix);
          }
          blocks.insert(pair<int, const Block*>(p->id(), p));
        } else {
          new_block.disband(matrix);
        }
      }
    }
  }
}

void Block::get_bound(int &min_x, int &max_x) const {
  min_x = WIDTH - 1;
  max_x = 0;
  for (int i = 0; i < _members.size(); ++i) {
    int x = _members[i].x;
    if (min_x > x) {
      min_x = x;
    }
    if (max_x < x) {
      max_x = x;
    }
  }
}
