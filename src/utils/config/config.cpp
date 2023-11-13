#include "utils/config/config.h"

#include <stdio.h>

void config_default(Config* config) {
  config->window_width = 600;
  config->window_height = 400;
  config->window_start_maximixed = true;
}

// TODO: Use JSON
Result config_save(const Config* config, const char* filepath) {
    FILE* file = fopen(filepath, "wb");
    if (file) {
        fwrite(config, sizeof(Config), 1, file);
        fclose(file);
        return RESULT_TYPE_SUCCESS;
    }
    else {
      Result result = {
        .msg = "Failed to save config. File didn't open.",
        .code = ResultCode::FAILURE_FILE_IO
      };

      return result;
    }
}

// TODO: Use JSON
Result config_load(Config* config, const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (file) {
        fread(config, sizeof(Config), 1, file);
        fclose(file);
        return RESULT_TYPE_SUCCESS;
    }
    else {
      Result result = {
        .msg = "Failed to load config. File didn't open.",
        .code = ResultCode::FAILURE_FILE_IO
      };

      return result;
    }
}

