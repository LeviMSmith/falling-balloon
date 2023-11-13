#include "core.h"
#include "app.h"

Result App::create(App*& app, Args* args) {
  app = new App;

  app->args = args;

  return RESULT_TYPE_SUCCESS;
}

void App::destroy(App*& app) {
  delete app;
}
