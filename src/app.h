#ifndef APP_H_
#define APP_H_

#include "core.h"

#include "utils/config/args.h"
#include "utils/config/config.h"

class App {
public:
  static Result create(App*& app, Args* args);
  static void destroy(App*& app);

private:
  Args* args;
  Config config;
};

#endif // APP_H_
