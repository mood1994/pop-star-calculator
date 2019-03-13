#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <vector>
#include "star.hpp"
#include "def.hpp"

class block {
 public:
  int id() { return _id; }

  void set_id(int id) { _id = id; }

  void add(star *s) {
    _members.push_back(s);
  }

  int member_count() {
    return _members.size();
  }

  void disband();

  void print(); 

 private:
  int _id;
  std::vector<star *> _members;
};

void init_blocks(star* star_matrix[WIDTH][LENGTH], std::vector<block> &blocks);

#endif // BLOCK_HPP
