#ifndef RESULT_H
#define RESULT_H

enum ResultCode {
  SUCCESS = 0,

  FAILURE_ARGS_PARSE,
  FAILURE_ARGS_HELP,

  FAILURE_FILE_IO,
};

typedef struct Result {
  const char* msg;
  ResultCode code;
} Result;

const extern Result RESULT_TYPE_SUCCESS;

#endif //RESULT_H
