#ifndef GL_H_
#define GL_H_

#include "core.h"

#include "update/ecs/ecs.h"
#include "render/mesh.h"

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct DrawInfo {
  std::unordered_map<EntityID, Mesh> updated_chunk_meshes;
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

  class ChunkPipeline {
  public:
    GLuint shader_program;
    GLuint vbo;
    GLuint vao;
    std::unordered_map<EntityID, Mesh> chunk_meshses;
    Result prepare();
    void cleanup();
    void update(const std::unordered_map<EntityID, Mesh>& chunk_meshes);
    void draw();
  };

  ChunkPipeline chunk_pipeline;
};

#endif
