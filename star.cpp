#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "star.hpp"

using namespace std;

const Star Star::INVALID;

void Star::init_stars(Star stars[STAR_COUNT]) {
  srand((unsigned)time(NULL));
  for (int i = 0; i < STAR_COUNT; ++i) {
    short rand_type = rand() % STAR_TYPE_COUNT;
    stars[i].set_type(rand_type);
    stars[i].set_id(i);
  }
}

int Star::read_stars_from_file(int argc, char **argv, Star stars[STAR_COUNT]) {
  int rc = 0;
  ifstream ifs;
  char c = 0;
  int i = 0;

  if (argc != 2) {
    cerr << "Wrong count of arguments" << endl;
    rc = 1;
    goto error;
  }

  ifs.open(argv[1], ifstream::in);
  if (ifs.fail()) {
    cerr << "Can't open file " << argv[1] << endl;
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
      if (type < 0 || type >= STAR_TYPE_COUNT) {
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

done:
  if (ifs.is_open()) {
    ifs.close();
  }
  return rc;
error:
  goto done;
}

void Star::init_matrix(Star stars[STAR_COUNT], Star star_matrix[WIDTH][LENGTH]) {
  for (int y = 0; y < LENGTH; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      star_matrix[x][y] = stars[x + y * WIDTH];
    }
  }
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

