#ifndef STAR_HPP
#define STAR_HPP

#include <iostream>
#include "def.hpp"
#include "util.hpp"

class Star {
 public:
  static const Star INVALID;

  static void make_random_matrix(Star matrix[WIDTH][LENGTH]);

  static int read_matrix_from_file(const char *file_path, 
                                   Star matrix[WIDTH][LENGTH]);
  
  static void print_matrix(const Star star_matrix[WIDTH][LENGTH]);

 public:
  Star() {
    _id = INVALID_STAR_ID;
    _type = INVALID_STAR_TYPE;
    _block_id = INVALID_BLOCK_ID;
  }

  short type() const { return _type; }

  void set_type(short type) { _type = type; }

  short id() const { return _id; }

  void set_id(short id) { _id = id; }

  int block_id() const { return _block_id; }

  void set_block_id(int block_id) { _block_id = block_id; }

  void print() const {
    std::cout << "{ id: " << _id << ", type: " << _type << " }" << std::endl;
  }

 private:
  short _id;
  short _type;
  int _block_id;
};

bool operator == (const Star &l, const Star &r);

bool operator != (const Star &l, const Star &r);



class Mini_matrix {
 private:
  static const uint MINI_MTRX_SIZE = DIVIDE_AND_CEIL(TYPE_BITS * STAR_COUNT, BYTE_BITS);

 public:
  Mini_matrix(const Star matrix[WIDTH][LENGTH], short score);

  Mini_matrix(const Mini_matrix& m);

  void print() const;

  bool operator <(const Mini_matrix &m) const;

  uint hash(uint max) const;

 private:
  byte _buf[MINI_MTRX_SIZE];
  short _score;
};

typedef std::pair<std::set<Mini_matrix>::iterator, bool> Mtrx_set_ret;
typedef Hash_set<Mini_matrix, Mtrx_set_ret, 1024> Mtrx_hash_set;

#endif // STAR_HPP
