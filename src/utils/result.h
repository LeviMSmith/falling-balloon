#ifndef RESULT_H
#define RESULT_H

enum Result: int {
  SUCCESS,

  FAILURE_ARGS_PARSE,
  FAILURE_ARGS_HELP,

  FAILURE_FILE_IO,

  FAILURE_POOL_FULL,

  FAILURE_UNKNOWN_COMPONENT_TYPE,
};

#endif //RESULT_H
