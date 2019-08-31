#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <vector>
#include "star.hpp"
#include "def.hpp"

class Block {
 public:
  static void init_blocks(Star* star_matrix[WIDTH][LENGTH], 
                          std::vector<Block> &blocks);

 public:
  int id() const { return _id; }

  void set_id(int id) { _id = id; }

  void add(Star *s) {
    _members.push_back(s);
  }

  const std::vector<Star *> &members() const { return _members; }

  void print() const;

 private:
  void disband();

 private:
  int _id;
  std::vector<Star *> _members;
};


#endif // BLOCK_HPP
