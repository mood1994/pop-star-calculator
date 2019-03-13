#ifndef STAR_HPP
#define STAR_HPP

#include <iostream>
#include "def.hpp"

class star {
 public:
  star() {
    _block_id = INVALID_BLOCK_ID;
  }

  int type() const { return _type; }

  void set_type(int type) { _type = type; }

  int id() const { return _id; }

  void set_id(int id) { _id = id; }

  int block_id() const { return _block_id; }

  void set_block_id(int block_id) { _block_id = block_id; }

  void print() {
    std::cout << "{ id: " << _id << ", type: " << _type << " }" << std::endl;
  }

 private:
  int _type;
  int _id;
  int _block_id;
};

#endif // STAR_HPP
