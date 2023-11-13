#include "core.h"
#include "app.h"
#include "utils/config/args.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    Args* args = nullptr;
    Args::create(args);
    Result parse_result = Args::parse(argc, argv, args);
    if (parse_result.code != ResultCode::SUCCESS) {
        printf("Arg parsing failed: %s\n", parse_result.msg);
        return 1;
    }

    const LogLevel log_level = args->log_level;
    Result log_init_result = init_log(log_level);
    if (log_init_result.code == ResultCode::SUCCESS) {
        LOG_INFO("Log initialized!");
    }
    else {
      return 1;
    }

    App* app = nullptr;
    Result app_create_result = App::create(app, args);
    if (app_create_result.code != ResultCode::SUCCESS) {
        LOG_FATAL("Couldn't create app:");
        LOG_FATAL(app_create_result.msg);
    }

    LOG_DEBUG("Debug");

    App::destroy(app);
    Args::destroy(args);

    return 0;
}
