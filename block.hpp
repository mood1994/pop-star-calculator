#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <vector>
#include "star.hpp"
#include "def.hpp"

struct Coord {
  Coord(short arg_x, short arg_y): x(arg_x), y(arg_y) {}
  short x;
  short y;
};

class Block {
 public:
  static short max_id;
  static void init_blocks(Star star_matrix[WIDTH][LENGTH], 
                          std::vector<Block> &blocks);

 public:
  short id() const { return _id; }

  void set_id(short id) { _id = id; }

  void add(const Coord &c) {
    _members.push_back(c);
  }

  const std::vector<Coord> &members() const { return _members; }

  void print() const;

 private:
  short _id;
  std::vector<Coord> _members;
};


#endif // BLOCK_HPP
