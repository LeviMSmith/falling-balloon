#ifndef GL_H_
#define GL_H_

#include "core.h"

#include "ecs/ecs.h"
#include "render/mesh.h"

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

struct DrawInfo {
  std::vector<Mesh> new_chunk_meshes;
  std::optional<glm::mat4> new_view;

  void clear();
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
    ChunkPipeline();
    ~ChunkPipeline();

    static GLuint shader_program;
    static GLuint model_location;
    static GLuint view_location;
    static GLuint projection_location;
    static GLuint texture_atlas;
    GLuint vbo;
    GLuint vao;
    glm::mat4 model;
    size_t num_verticies;

    void update(const Mesh& mesh);
    void draw() const;

    static Result create_shader_program();
  };

  std::vector<ChunkPipeline> chunk_pipelines;
};

#endif
