#ifndef STAR_HPP
#define STAR_HPP

#include <iostream>
#include "def.hpp"

class Star {
 public:
  static const Star INVALID;
  static void init_stars(Star stars[STAR_COUNT]);

  static int read_stars_from_file(int argc, 
                                  char **argv, 
                                  Star stars[STAR_COUNT]);
  
  static void init_matrix(Star stars[STAR_COUNT], 
                          Star star_matrix[WIDTH][LENGTH]);

  static void print_matrix(const Star star_matrix[WIDTH][LENGTH]);

 public:
  Star() {
    _id = INVALID_STAR_ID;
    _type = INVALID_STAR_TYPE;
    _block_id = INVALID_BLOCK_ID;
  }

  short type() const { return _type; }

  void set_type(short type) { _type = type; }

  int id() const { return _id; }

  void set_id(int id) { _id = id; }

  short block_id() const { return _block_id; }

  void set_block_id(short block_id) { _block_id = block_id; }

  void print() const {
    std::cout << "{ id: " << _id << ", type: " << _type << " }" << std::endl;
  }

 private:
  int _id;
  short _type;
  short _block_id;
};

bool operator == (const Star &l, const Star &r);

bool operator != (const Star &l, const Star &r);

#endif // STAR_HPP
