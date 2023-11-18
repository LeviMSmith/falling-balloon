#include "core.h"
#include "app.h"
#include "utils/config/args.h"
#include "utils/config/config.h"

#include "update/update.h"
#include "update/ecs/ecs.h"

#include "render/render.h"

Result App::create(App*& app, Args* args) {
  app = new App;
  app->config = nullptr;
  app->update = nullptr;

  app->args = args;
  Result config_create_res = Config::create(app->config);
  if (config_create_res != Result::SUCCESS) {
    App::destroy(app);
    app = nullptr;
    return config_create_res;
  }

  Result update_create_res = Update::create(app->update);
  if (update_create_res != Result::SUCCESS) {
    App::destroy(app);
    app = nullptr;
    return update_create_res;
  }



  return Result::SUCCESS;
}

void App::destroy(App* app) {
  Update::destroy(app->update);
  Config::destroy(app->config);
  // app->args is handled in main.cpp. App just has a handle.
  if (app != nullptr) {
    delete app;
  }
}
