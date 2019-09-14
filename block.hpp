#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "star.hpp"
#include "def.hpp"
#include "util.hpp"

struct Coord {
  short x;
  short y;

  Coord(short arg_x, short arg_y): x(arg_x), y(arg_y) {}

  bool operator ==(const Coord &c) const {
    return x == c.x && y == c.y;
  }

  bool operator <(const Coord &c) const {
    return x < c.x || y < c.y;
  }

  bool operator >(const Coord &c) const {
    return x > c.x || y > c.y;
  }
};



class Block;
typedef std::pair<std::set<Block>::iterator, bool> Block_set_ret;
typedef Hash_set<Block, Block_set_ret, 128> Block_hash_set;

class Block {
 public:
  static Block_hash_set global_blocks;
  static void org_blocks(Star star_matrix[WIDTH][LENGTH], 
                         int min_x, int max_x,
                         int min_y, int max_y,
                         std::map<int, const Block*> &blocks);

 public:
  Block(): _id(INVALID_BLOCK_ID), _type(INVALID_STAR_TYPE) {}

  int id() const { return _id; }

  void set_id(int id) { _id = id; }

  short type() const { return _type; }

  void set_type(short t) { _type = t; }

  void add(const Coord &c) {
    _members.push_back(c);
  }

  void disband(Star star_matrix[WIDTH][LENGTH]) const;

  const std::vector<Coord> &members() const { return _members; }

  void get_bound_x(int &min_x, int &max_x) const;

  void get_bound_y(int &min_y, int &max_y) const;

  void print() const;

  bool operator <(const Block &b) const;

  uint hash(uint max) const;

 private:
  int _id;
  short _type;
  std::vector<Coord> _members;
};

#endif // BLOCK_HPP
