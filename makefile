PROJECTNAME := app

# Directories
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin

# Compiler options (default to clang)
COMPILER ?= clang++

# Build type (default to release)
BUILD_TYPE := debug
ifeq ($(BUILD), release)
    BUILD_TYPE := release
endif

# Compiler and linker flags
CFLAGS := -Isrc -MMD -Wall -Wunused-result -std=c++20
LDFLAGS :=
ifeq ($(BUILD_TYPE), release)
    CFLAGS += -O2 -DNDEBUG
else
    CFLAGS += -g
endif

# Find all the source files in the src directory and its subdirectories
SOURCES := $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)
TARGET := $(TARGETDIR)/$(PROJECTNAME)

# Default target is to build the program
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(COMPILER) $^ -o $@ $(LDFLAGS)

# Pattern rule to build object files from source files
$(OBJECTS): $(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -c $< -o $@


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
