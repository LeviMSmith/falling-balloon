#include "core.h"
#include "utils/config/args.h"

#include <getopt.h>
#include <stdio.h>

const char* usage_message = "\
Usage: program_name [-c config_location] [-v] [-h]\n\
\n\
Options:\n\
  -c config_location     Specify the location of the configuration file.\n\
                         If not provided, a default location will be used.\n\
\n\
  -v                     Enable verbose logging with debug level messages.\n\
\n\
  -h                     Display this help message and exit.\n\
";

void print_usage() {
  printf("%s", usage_message);
}

Result Args::create(Args*& args) {
  args = new Args;

  args->log_level = LogLevel::INFO;

  return Result::SUCCESS;
}

void Args::destroy(Args*& args) {
  delete args;
}

Result Args::parse(int argc, char* const argv[], Args* args) {
  int opt;
  while ((opt = getopt(argc, argv, "c::hv")) != -1) {
    switch (opt) {
      case 'c': {
        args->config_location = optarg;
        break;
      }
      case 'v': {
        args->log_level = LogLevel::DEBUG;
        break;
      }
      case 'h': {
        print_usage();
        return Result::FAILURE_ARGS_HELP;
      }
      default: {
        print_usage();
        return Result::FAILURE_ARGS_PARSE;
      }
    }
  }

  return Result::SUCCESS;
}
