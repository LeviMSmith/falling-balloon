#ifndef RESULT_H
#define RESULT_H

enum Result: int {
  SUCCESS,

  FAILURE_ARGS_PARSE,
  FAILURE_ARGS_HELP,

  FAILURE_FILE_IO,
};

#endif //RESULT_H
