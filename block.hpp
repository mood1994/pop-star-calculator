#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <vector>
#include "star.hpp"
#include "def.hpp"

class Block {
 public:
  int id() { return _id; }

  void set_id(int id) { _id = id; }

  void add(Star *s) {
    _members.push_back(s);
  }

  int member_count() {
    return _members.size();
  }

  void disband();

  void print(); 

  static void init_blocks(Star* star_matrix[WIDTH][LENGTH], std::vector<Block> &blocks);

 private:
  int _id;
  std::vector<Star *> _members;
};


#endif // BLOCK_HPP
