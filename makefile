PROJECTNAME := falling-balloon

# Directories
SRCDIR := sauce
BUILDDIR := $(CURDIR)/build
APPBUILDDIR := $(BUILDDIR)/app
TARGETDIR := bin

GLFW_DIR := vendor/glfw
GLFW_BUILD_DIR := $(BUILDDIR)/glfw
GLFW_LIB := $(GLFW_BUILD_DIR)/src/libglfw3.a
GLFW_INCLUDE := $(GLFW_DIR)/include

GLEW_DIR := vendor/glew-2.2.0
GLEW_LIB := $(GLEW_DIR)/lib/libGLEW.a
GLEW_INCLUDE := $(GLEW_DIR)/include

# Compiler options (default to clang)
CC := /usr/bin/clang
CXX := /usr/bin/clang++

# Build type (default to release)
BUILD_TYPE := debug
ifeq ($(BUILD), release)
    BUILD_TYPE := release
endif

CMAKE_BUILD_TYPE := $(shell echo $(BUILD_TYPE) | awk '{print toupper(substr($$0, 1, 1)) tolower(substr($$0, 2))}')

# Compiler and linker flags
CFLAGS := -I$(SRCDIR) -I$(GLFW_INCLUDE) -I$(GLEW_INCLUDE) -MMD -Wall -Wunused-result -std=c++20
LDFLAGS := -lX11 -lGL
ifeq ($(BUILD_TYPE), release)
    CFLAGS += -O2 -DNDEBUG
else
    CFLAGS += -g
endif

# Find all the source files in the src directory and its subdirectories
SOURCES := $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(APPBUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET := $(TARGETDIR)/$(PROJECTNAME)

# Default target is to build the program
all: $(TARGET)

$(TARGET): $(OBJECTS) $(GLFW_LIB) $(GLEW_LIB) | $(TARGETDIR)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Pattern rule to build object files from source files
$(OBJECTS): $(APPBUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(GLFW_LIB): cmake_config | $(GLFW_BUILD_DIR)
	$(MAKE) -C $(GLFW_BUILD_DIR)

$(GLEW_LIB):
	$(MAKE) -C $(GLEW_DIR) glew.lib.static

$(BUILDDIR) $(GLFW_BUILD_DIR) $(TARGETDIR) $(TARGETLIBDIR):
	mkdir -p $@

.PHONY: cmake_config
cmake_config: | $(GLFW_BUILD_DIR)
	cd $(GLFW_DIR) && cmake -S . -B $(GLFW_BUILD_DIR) -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

# Include the dependency files
-include $(DEPS)

# Clean target
clean:
	@echo " Cleaning... "
	rm -rf $(BUILDDIR) $(TARGETDIR)
	rm -rf .cache
	rm -f $(GLEW_LIB)
	@echo " Done. "

run: all
	@./$(TARGET)

# Phony targets
.PHONY: all clean
