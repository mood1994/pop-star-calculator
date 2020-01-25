#ifndef CONF_HPP
#define CONF_HPP

#include <stdlib.h>

class Config {
 public:
  Config(): file(NULL), debug(false), thd_num(1) {}

  void parse_conf(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i], "--debug") == 0 ||
          strcmp(argv[i], "-d") == 0) {
        debug = true;
        continue;
      }
      if (strcmp(argv[i], "--file") == 0 ||
          strcmp(argv[i], "-f") == 0) {
        if (i + 1 < argc) {
          file = argv[i + 1];
          i += 1;
        }
      }
      if (strcmp(argv[i], "--thread") == 0 ||
          strcmp(argv[i], "-t") == 0) {
        if (i + 1 < argc) {
          thd_num = atoi(argv[i + 1]);
          i += 1;
        }
      }
    }
  }

  char *file;
  bool debug;
  int thd_num;
};

extern Config g_conf;

#endif // CONF_HPP
