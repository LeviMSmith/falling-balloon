#include "utils/config/config.h"

#include <stdio.h>

Result Config::create(Config*& config) {
    config = new Config;
    config->window_width = 600;
    config->window_height = 400;
    config->window_start_maximixed = true;

    return Result::SUCCESS;
}

void Config::destroy(Config* config) {
    delete config;
}

// TODO: Use JSON
Result Config::save(const Config* config, const char* filepath) {
    FILE* file = fopen(filepath, "wb");
    if (file) {
        fwrite(config, sizeof(Config), 1, file);
        fclose(file);
        return Result::SUCCESS;
    }
    else {
      return Result::FAILURE_FILE_IO;
    }
}

// TODO: Use JSON
Result Config::load(Config* config, const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (file) {
        fread(config, sizeof(Config), 1, file);
        fclose(file);
        return Result::SUCCESS;
    }
    else {
      return Result::FAILURE_FILE_IO;
    }
}

