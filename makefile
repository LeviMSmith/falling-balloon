PROJECTNAME := falling-balloon

# Directories
SRCDIR := sauce
BUILDDIR := $(CURDIR)/build
APPBUILDDIR := $(BUILDDIR)/app
TARGETDIR := bin
TARGETLIBDIR := $(TARGETDIR)/lib

GLFW_DIR := vendor/glfw
GLFW_BUILD_DIR := $(BUILDDIR)/glfw
GLFW_LIB := $(GLFW_BUILD_DIR)/src/libglfw3.a
GLFW_INCLUDE := $(GLFW_DIR)/include

DILIGENT_DIR := vendor/DiligentEngine
DILIGENT_BUILD_DIR := $(BUILDDIR)/DiligentEngine
DILIGENT_SHARED_LIBS_FILENAMES := libArchiver.so libGraphicsEngineVk.so libGraphicsEngineOpenGL.so
DILIGENT_SHARED_LIBS := $(addprefix $(TARGETLIBDIR), DILIGENT_SHARED_LIBS_FILENAMES)
DILIGENT_LIBS_FILENAMES := libglew-static.a libOGLCompiler.a libspirv-cross-core.a libDiligentCore.a libglslang.a libHLSL.a libOSDependent.a libSPIRV-Tools.a libGenericCodeGen.a libMachineIndependent.a libSPIRV.a libSPIRV-Tools-opt.a
DILIGENT_LIBS := $(addprefix $(DILIGENT_BUILD_DIR)/install/lib/DiligentCore/$(CMAKE_BUILD_TYPE), $(DILIGENT_LIBS_FILENAMES))
DILIGENT_INCLUDE := $(DILIGENT_BUILD_DIR)/install/include/DiligentCore

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
CFLAGS := -I$(SRCDIR) -I$(GLFW_INCLUDE) -MMD -Wall -Wunused-result -std=c++20
LDFLAGS := -lX11 -L$(TARGETLIBDIR)
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

$(TARGET): $(OBJECTS) $(GLFW_LIB) $(DILIGENT_LIBS) | $(TARGETDIR)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Pattern rule to build object files from source files
$(OBJECTS): $(APPBUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(GLFW_LIB): cmake_config | $(GLFW_BUILD_DIR)
	$(MAKE) -C $(GLFW_BUILD_DIR)

$(DILIGENT_LIBS): cmake_config | $(DILIGENT_BUILD_DIR)
	cmake --build $(DILIGENT_BUILD_DIR) --target install

$(DILIGENT_SHARED_LIBS): $(DILIGENT_LIBS) | $(TARGETLIBDIR)
	@cp $(addprefix $(DILIGENT_BUILD_DIR)/install/lib/DiligentCore/$(CMAKE_BUILD_TYPE), $(DILIGENT_SHARED_LIBS_FILENAMES)) $(TARGETLIBDIR)

$(BUILDDIR) $(GLFW_BUILD_DIR) $(DILIGENT_BUILD_DIR) $(TARGETDIR) $(TARGETLIBDIR):
	mkdir -p $@

.PHONY: cmake_config
cmake_config: | $(GLFW_BUILD_DIR) $(DILIGENT_BUILD_DIR)
	cd $(GLFW_DIR) && cmake -S . -B $(GLFW_BUILD_DIR) -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
	cd $(DILIGENT_DIR) && cmake -S . -B $(DILIGENT_BUILD_DIR) -G "Unix Makefiles" -DDILIGENT_BUILD_TOOLS=OFF -DDILIGENT_BUILD_FX=OFF -DDILIGENT_BUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX)

# Include the dependency files
-include $(DEPS)

# Clean target
clean:
	@echo " Cleaning... "
	rm -rf $(BUILDDIR) $(TARGETDIR)
	rm -rf .cache
	@echo " Done. "

run: all
	@./$(TARGET)

# Phony targets
.PHONY: all clean
