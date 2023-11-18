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

# Compiler options (default to clang)
COMPILER ?= clang++

# Build type (default to release)
BUILD_TYPE := debug
ifeq ($(BUILD), release)
    BUILD_TYPE := release
endif

# Compiler and linker flags
CFLAGS := -I$(SRCDIR) -I$(GLFW_INCLUDE) -MMD -Wall -Wunused-result -std=c++20
LDFLAGS := -lX11
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

$(TARGET): $(OBJECTS) $(GLFW_LIB)
	@mkdir -p $(@D)
	$(COMPILER) $^ -o $@ $(LDFLAGS)

# Pattern rule to build object files from source files
$(OBJECTS): $(APPBUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -c $< -o $@

$(GLFW_LIB):
	@mkdir -p $(GLFW_BUILD_DIR)
	cd $(GLFW_DIR) && \
		cmake -S . -B $(GLFW_BUILD_DIR) -DBUILD_SHARED_LIBS=off -DGLFW_BUILD_EXAMPLES=off -DGLFW_BUILD_DOCS=off
	make -C $(GLFW_BUILD_DIR)

# Include the dependency files
-include $(DEPS)

# Clean target
clean:
	@echo " Cleaning... "
	rm -rf $(BUILDDIR) $(TARGETDIR)
	@echo " Done. "

run: all
	@./$(TARGET)

# Phony targets
.PHONY: all clean
