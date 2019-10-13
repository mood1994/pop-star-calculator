#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "star.hpp"

using namespace std;

const Star Star::INVALID;

Mtrx_hash_set Mini_matrix::g_hash_set;

void Star::make_random_matrix(Star matrix[WIDTH][LENGTH]) {
  int x = 0;
  int y = 0;
  srand((unsigned)time(NULL));
  for (int i = 0; i < STAR_COUNT; ++i) {
    short rand_type = (rand() % STAR_TYPE_COUNT) + 1;
    matrix[x][y].set_type(rand_type);
    matrix[x][y].set_id(i);
    y += 1;
    if (y == LENGTH) {
      x += 1;
    }
  }
}

int Star::read_matrix_from_file(const char *file_path, Star matrix[WIDTH][LENGTH]) {
  int rc = 0;
  ifstream ifs;
  char c = 0;
  int i = 0;
  Star stars[STAR_COUNT];

  ifs.open(file_path, ifstream::in);
  if (ifs.fail()) {
    cerr << "Can't open file " << file_path << endl;
    rc = 1;
    goto error;
  }

  c = ifs.get();
  while (ifs.good()) {
    if ('\n' == c || '\r' == c) {
      if (i % WIDTH != 0) {
        cerr << "Wrong format! Unexpected width" << endl;
        rc = 1;
        goto error;
      }

    } else {
      int type = c - '0';
      if (type < 1 || type > STAR_TYPE_COUNT) {
        cerr << "Wrong format! Unexpected type" << endl;
        rc = 1;
        goto error;
      }
      stars[i].set_id(i);
      stars[i].set_type(type);
      ++i;
    }

    c = ifs.get();
  }

  if (i != STAR_COUNT) {
    cerr << "Wrong stars count " << i << endl;
    rc = 1;
    goto error;
  }

  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      matrix[x][y] = stars[x + y * WIDTH];
    }
  }

done:
  if (ifs.is_open()) {
    ifs.close();
  }
  return rc;
error:
  goto done;
}

void Star::print_matrix(const Star star_matrix[WIDTH][LENGTH]) {
  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      short t = star_matrix[x][y].type();
      if (t != INVALID_STAR_TYPE) {
        cout << t;
      } else {
        cout << ' ';
      }
    }
    cout << endl;
  }
  cout << endl;
}

bool operator == (const Star &l, const Star &r) {
  return l.id() == r.id();
}

bool operator != (const Star &l, const Star &r) {
  return !operator==(l, r);
}



Mini_matrix::Mini_matrix(const Star matrix[WIDTH][LENGTH], short score) {
  int i = 0;
  int x = 0;
  int y = 0;
  uint offset = 0;
  ullong *buf_bits = (ullong*) _buf;

  memset(_buf, 0, sizeof(_buf));
  for (int i = 1; i <= STAR_COUNT; ++i) {
    ullong t = matrix[x][y].type();
    *buf_bits |= (t << offset);

    y += 1;
    if (y == LENGTH) {
      y = 0;
      x += 1;
    }

    offset += TYPE_BITS;
    if (i % BYTE_BITS == 0) {
      offset = 0;
      buf_bits = (ullong*) (_buf + ((i / BYTE_BITS) * TYPE_BITS));
    }
  }

  _score = score;
}

Mini_matrix::Mini_matrix(const Mini_matrix& m) {
  memcpy(_buf, m._buf, sizeof(_buf));
  _score = m._score;
}

void Mini_matrix::print() const {
  cout << "score: " << _score << endl;
  for (int i = 0; i < MINI_MTRX_SIZE; ++i) {
    byte b = _buf[i];
    for (int j = 0; j < BYTE_BITS; ++j) {
      cout << ((b & (1 << (BYTE_BITS - 1 - j))) ? '1' : '0');
    }
    cout << ' ';
    if (i % 8 == 7) {
      cout << endl;
    }
  }
  cout << endl;
}

bool Mini_matrix::operator <(const Mini_matrix &m) const {
  return memcmp(_buf, m._buf, sizeof(_buf)) < 0 || _score < m._score;
}

uint Mini_matrix::hash(uint max) const {
  ullong hash_code = _score;
  const ullong *p = (const ullong *) _buf;
  for (uint i = 0; i < MINI_MTRX_SIZE / sizeof(ullong); ++i) {
    hash_code += *p;
    p += 1;
  }
  hash_code %= max;
  return hash_code;
}
