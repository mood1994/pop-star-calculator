#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "star.hpp"

using namespace std;

void init_stars(star stars[STAR_COUNT]) {
  srand((unsigned)time(NULL));
  for (int i = 0; i < STAR_COUNT; ++i) {
    int rand_type = rand() % STAR_TYPE_COUNT;
    stars[i].set_type(rand_type);
    stars[i].set_id(i);
  }
}

int read_stars_from_file(int argc, char **argv, star stars[STAR_COUNT]) {
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

