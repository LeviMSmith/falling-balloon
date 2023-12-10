PROJECTNAME := falling-balloon

BUILD_TYPE := debug
ifeq ($(BUILD), release)
    BUILD_TYPE := release
endif

# For vendor builds
CMAKE_BUILD_TYPE := $(shell echo $(BUILD_TYPE) | awk '{print toupper(substr($$0, 1, 1)) tolower(substr($$0, 2))}')

# Directories
SRCDIR := sauce
BUILDDIR := $(CURDIR)/build/$(CMAKE_BUILD_TYPE)
APPBUILDDIR := $(BUILDDIR)/app
TARGETDIR := bin/$(BUILD_TYPE)
RESOURCEDIR := resources

# Resources
RESOURCE_FILES := $(shell find $(RESOURCEDIR) -type f)

# GLFW
GLFW_DIR := vendor/glfw
GLFW_BUILD_DIR := $(BUILDDIR)/glfw
GLFW_LIB := $(GLFW_BUILD_DIR)/src/libglfw3.a
GLFW_INCLUDE := $(GLFW_DIR)/include

# GLEW
GLEW_DIR := vendor/glew-2.2.0
GLEW_LIB := $(GLEW_DIR)/lib/libGLEW.a
GLEW_INCLUDE := $(GLEW_DIR)/include

# GLM
GLM_DIR := vendor/glm
GLM_INCLUDE := $(GLM_DIR)

# STB
STB_DIR := vendor/stb

# Compiler options (default to clang)
CC := /usr/bin/clang
CXX := /usr/bin/clang++

# Compiler and linker flags
CFLAGS := -I$(SRCDIR) -I$(STB_DIR) -I$(GLFW_INCLUDE) -I$(GLEW_INCLUDE) -I$(GLM_INCLUDE) -MMD -Wall -Wunused-result -std=c++20
DFLAGS := -DPLATFORM_LINUX -DGLEW_STATIC -DGLFW_INCLUDE_NONE -DRESOURCE_DIR=\"$(RESOURCEDIR)\"
LDFLAGS := -lX11 -lGL
ifeq ($(BUILD_TYPE), release)
    CFLAGS += -O2
    DFLAGS += -DNDEBUG
else
    CFLAGS += -g
endif

# Find all the source files in the src directory and its subdirectories
SOURCES := $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(APPBUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET := $(TARGETDIR)/$(PROJECTNAME)

# Default target is to build the program
.PHONY: all
all: build

.PHONY: build
build: $(TARGET) resources

$(TARGET): $(OBJECTS) $(GLFW_LIB) $(GLEW_LIB) | $(TARGETDIR)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Pattern rule to build object files from source files
$(OBJECTS): $(APPBUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(GLFW_LIB): cmake_config | $(GLFW_BUILD_DIR)
	$(MAKE) -C $(GLFW_BUILD_DIR)

$(GLEW_LIB):
	$(MAKE) -C $(GLEW_DIR) glew.lib.static

$(BUILDDIR) $(GLFW_BUILD_DIR) $(TARGETDIR) $(RESOURCETARGETDIR):
	mkdir -p $@

.PHONY: cmake_config
cmake_config: | $(GLFW_BUILD_DIR)
	@cd $(GLFW_DIR) && cmake -S . -B $(GLFW_BUILD_DIR) -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)

.PHONY: resources
resources: $(RESOURCE_FILES) | $(TARGETDIR)
	@echo "Copying resources..."
	@$(foreach file,$^,cp --parents $(file) $(TARGETDIR);)

# Include the dependency files
-include $(DEPS)

# Clean target
.PHONY: clean
clean:
	@echo " Cleaning... "
	rm -rf $(BUILDDIR) $(TARGETDIR)
	rm -rf .cache
	rm -f $(GLEW_LIB)
	@echo " Done. "

.PHONY: run
run: all
	@./$(TARGET)
