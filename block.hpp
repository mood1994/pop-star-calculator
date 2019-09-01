#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <vector>
#include <map>
#include "star.hpp"
#include "def.hpp"

struct Coord {
  Coord(short arg_x, short arg_y): x(arg_x), y(arg_y) {}
  short x;
  short y;
};

class Block {
 public:
  static int max_id;
  static void init_blocks(Star star_matrix[WIDTH][LENGTH], 
                          std::map<int, Block> &blocks);

 public:
  int id() const { return _id; }

  void set_id(int id) { _id = id; }

  void add(const Coord &c) {
    _members.push_back(c);
  }

  void disband(Star star_matrix[WIDTH][LENGTH]) const;

  const std::vector<Coord> &members() const { return _members; }

  void print() const;

 private:
  int _id;
  std::vector<Coord> _members;
};


#endif // BLOCK_HPP
