#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "core.h"

struct Args {
  static Result create(Args*& args);
  static void destroy(Args*& args);
  static Result parse(int argc, char* const argv[], Args* args);

  LogLevel log_level;
  const char *config_location;
};


#endif // ARGPARSE_H
