#include "core.h"
#include "app.h"
#include "utils/config/args.h"
#include "utils/config/config.h"

#include "update/update.h"
#include "update/ecs/ecs.h"

#include "render/render.h"

#include "event/event.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

Result App::create(App*& app, Args* args) {
  app = new App;
  app->config = nullptr;
  app->update = nullptr;
  app->render = nullptr;
  app->event_handler = nullptr;

  app->args = args;
  Result config_create_res = Config::create(app->config);
  if (config_create_res != Result::SUCCESS) {
    LOG_FATAL("App failed to create config.");
    App::destroy(app);
    return config_create_res;
  }

  Result update_create_res = Update::create(app->update);
  if (update_create_res != Result::SUCCESS) {
    LOG_FATAL("App failed to create updater.");
    App::destroy(app);
    return update_create_res;
  }

  Result render_create_res = Render::create(app->render, app->config);
  if (render_create_res != Result::SUCCESS) {
    LOG_FATAL("App failed to create renderer.");
    App::destroy(app);
    return render_create_res;
  }

  GLFWwindow* glfw_window = nullptr;
  app->render->get_glfw_window(glfw_window);

  Result event_create_res = EventHandler::create(app->event_handler, glfw_window);
  if (event_create_res != Result::SUCCESS) {
    LOG_FATAL("App failed to create event handler.");
    App::destroy(app);
    return event_create_res;
  }

  return Result::SUCCESS;
}

void App::destroy(App*& app) {
  Render::destroy(app->render);
  Update::destroy(app->update);
  Config::destroy(app->config);
  // app->args is handled in main.cpp. App just has a handle.
  if (app != nullptr) {
    delete app;
  }
  app = nullptr;
}
