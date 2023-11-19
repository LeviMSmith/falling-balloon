#ifndef GL_H_
#define GL_H_

#include "core.h"

#include "update/ecs/ecs.h"
#include "render/mesh.h"

#include <filesystem>
#include <string>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct DrawInfo {
  std::vector<Mesh> chunk_meshes;
};

enum GLSLbufferType {
  VERTEX,
};

struct GLSLbuffer {
  GLuint buffer;
  GLuint attribute_object;
  GLSLbufferType type;
};

struct Pipeline {
  GLuint shader_program;
  std::vector<GLSLbuffer> buffers;
};

class GlBackend {
public:
  static Result create(GlBackend*& gl_backend, GLFWwindow* glfw_window);
  static void destroy(GlBackend*& gl_backend);

  Result draw(DrawInfo& draw_info);
  Result present();

  void handle_resize(int width = 0, int height = 0);
private:
  GLFWwindow* glfw_window;

  static Result load_shader_source(std::string& source, std::filesystem::path shader_path);
  static Result compile_shader(GLuint& shader, const char* source, GLenum type);

  Pipeline chunk_pipeline;
  Result prepare_chunk_pipeline();
  void cleanup_chunk_pipeline();
  void draw_chunk_components(const std::vector<Mesh>& chunk_meshes);
};

#endif
