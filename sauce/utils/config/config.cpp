#include "utils/config/config.h"

#include <stdio.h>

Result Config::create(Config*& config) {
    config = new Config;
    config->window_width = 600;
    config->window_height = 400;
    config->window_start_maximixed = true;
    config->fps_cap = 60;

    return Result::SUCCESS;
}

void Config::destroy(Config* config) {
    if (config != nullptr) {
        // config->save();
        delete config;
    }
}

// TODO: Use JSON
Result Config::save(const char* filepath) const {
    FILE* file = fopen(filepath, "wb");
    if (file) {
        fwrite(this, sizeof(Config), 1, file);
        fclose(file);
        return Result::SUCCESS;
    }
    else {
      return Result::FAILURE_FILE_IO;
    }
}

// TODO: Use JSON
Result Config::load(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (file) {
        fread(this, sizeof(Config), 1, file);
        fclose(file);
        return Result::SUCCESS;
    }
    else {
      return Result::FAILURE_FILE_IO;
    }
}

