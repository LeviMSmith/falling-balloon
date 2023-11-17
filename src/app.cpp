#include "core.h"
#include "app.h"
#include "utils/config/args.h"
#include "utils/config/config.h"

Result App::create(App*& app, Args* args) {
  app = new App;

  app->args = args;
  Result config_result = Config::create(app->config);
  if (config_result != Result::SUCCESS) {
    App::destroy(app);
    app = nullptr;
    return config_result;
  }

  return Result::SUCCESS;
}

void App::destroy(App* app) {
  // app->args is handled in main.cpp. App just has a handle.
  Config::destroy(app->config);
  delete app;
}
