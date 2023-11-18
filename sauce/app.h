#ifndef APP_H_
#define APP_H_

#include "core.h"

#include "utils/config/args.h"
#include "utils/config/config.h"

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

  Update* update;
  Render* render;
  EventHandler* event_handler;
  Events events;
};

#endif // APP_H_
