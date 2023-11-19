#include "core.h"
#include "app.h"
#include "utils/config/args.h"
#include "utils/threadpool.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    Args* args = nullptr;
    Args::create(args);
    Result parse_result = Args::parse(argc, argv, args);
    if (parse_result != Result::SUCCESS) {
        printf("Arg parsing failed: %d\n", parse_result);
        return parse_result;
    }

    const LogLevel log_level = args->log_level;
    Result log_init_result = init_log(log_level);
    if (log_init_result == Result::SUCCESS) {
        LOG_INFO("Log initialized!");
    }
    else {
      return log_init_result;
    }

    ThreadPool::start();

    App* app = nullptr;
    Result app_create_result = App::create(app, args);
    if (app_create_result != Result::SUCCESS) {
        LOG_FATAL("Couldn't create app! Exiting.");
        return app_create_result;
    }

    LOG_INFO(argv[0]);

    Result app_run_result = app->run();

    ThreadPool::stop();
    App::destroy(app);
    Args::destroy(args);

    return app_run_result;
}
