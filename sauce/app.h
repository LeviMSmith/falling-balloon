#ifndef APP_H_
#define APP_H_

#include "core.h"

#include "utils/config/args.h"
#include "utils/config/config.h"
#include "utils/threadpool.h"

#include "update/update.h"
#include "render/render.h"
#include "event/event.h"

class App {
public:
  static Result create(App*& app, Args* args);
  static void destroy(App*& app);

  Result run();

private:
  Args* args;
  Config* config;

  ECS* ecs;
  Update* update;
  Render* render;
  EventHandler* event_handler;
  WindowEvents window_events;
  RenderEvents render_events;
};

#endif // APP_H_
