#include "core.h"
#include "render/gl.h"

#include "ecs/ecs.h"
#include "utils/resources.h"

#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

void DrawInfo::clear() {
  new_chunk_meshes.clear();
}

Result GlBackend::create(GlBackend*& gl_backend, GLFWwindow* glfw_window) {
  gl_backend = new GlBackend;
  gl_backend->glfw_window = glfw_window;

  glfwMakeContextCurrent(glfw_window);

  // Can I just say that I hate stateful libraries
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    GlBackend::destroy(gl_backend);
    LOG_ERROR("GlBackend failed to initialize GLEW:");
    LOG_ERROR((const char*)glewGetErrorString(err));
    return Result::FAILURE_GLEW_ERROR;
  }

  gl_backend->handle_resize();

  Result chunk_pipeline_prepare_res = ChunkPipeline::create_shader_program();
  if (chunk_pipeline_prepare_res != Result::SUCCESS) {
    GlBackend::destroy(gl_backend);
    return chunk_pipeline_prepare_res;
  }

  return Result::SUCCESS;
}

void GlBackend::destroy(GlBackend*& gl_backend) {
  if (gl_backend != nullptr) {
    delete gl_backend;
    gl_backend = nullptr;
  }
}

Result GlBackend::draw(DrawInfo& draw_info) {
  glClear(GL_COLOR_BUFFER_BIT);

  size_t num_new_chunks = draw_info.new_chunk_meshes.size();
  if (num_new_chunks > 0) {
    chunk_pipelines.resize(num_new_chunks);
    for (size_t i = 0; i < num_new_chunks; i++) {
      chunk_pipelines[i].update(draw_info.new_chunk_meshes[i]);
    }
  }

  if (draw_info.new_view.has_value() && ChunkPipeline::shader_program != 0) {
    glUniformMatrix4fv(ChunkPipeline::view_location, 1, GL_FALSE, glm::value_ptr(draw_info.new_view.value()));
  }

  for (const ChunkPipeline& chunk_pipeline : chunk_pipelines) {
    chunk_pipeline.draw();
  }

  return Result::SUCCESS;
}

Result GlBackend::present() {
  glfwSwapBuffers(glfw_window);
  return Result::SUCCESS;
}

void GlBackend::handle_resize(int width, int height) {
  LOG_INFO("GL backend handling framebuffer resize");

  // It is possible that GLFW hands us these values from the callback,
  // but just to make sure it's not the default we'll grab the values
  // from glfw again.
  if (width == 0 || height == 0) {
    LOG_DEBUG("Grabbing framebuffer size from GLFW");
    glfwGetFramebufferSize(glfw_window, &width, &height);
  }

  glViewport(0, 0, width, height);
  f32 aspect_ratio = (f32)width/(f32)height;
  glm::mat4 projection = glm::perspective(glm::radians(120.0f), aspect_ratio, 1.0f, 100.0f);
  glUniformMatrix4fv(ChunkPipeline::projection_location, 1, GL_FALSE, glm::value_ptr(projection));
}

Result GlBackend::load_shader_source(std::string& source, std::filesystem::path shader_path) {
  std::ifstream shader_file;

  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shader_file.open(shader_path);
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    source = shader_stream.str();
  } catch (std::ifstream::failure& e) {
    LOG_ERROR("Failed to read shader source");
    LOG_ERROR(shader_path.c_str());
    LOG_ERROR(e.what());
  }
  return Result::SUCCESS;
}

Result GlBackend::compile_shader(GLuint& shader, const char* source, GLenum type) {
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check for shader compile errors
  GLint success;
  GLchar info_log[1024];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 1024, NULL, info_log);
    LOG_ERROR("Failed to compile glsl source");
    LOG_ERROR(info_log);
    return Result::FAILURE_GLSL_SHADER_COMPILE;
  }

  return Result::SUCCESS;
}

//////////////////////////////////////
/// ChunkPipeline ////////////////////
//////////////////////////////////////

GLuint GlBackend::ChunkPipeline::shader_program = 0;
GLuint GlBackend::ChunkPipeline::model_location = 0;
GLuint GlBackend::ChunkPipeline::view_location = 0;
GLuint GlBackend::ChunkPipeline::projection_location = 0;

GlBackend::ChunkPipeline::ChunkPipeline() {
  if (ChunkPipeline::shader_program == 0) {
    ChunkPipeline::create_shader_program();
  }

  // Now the objects the pipeline uses
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, position));
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

GlBackend::ChunkPipeline::~ChunkPipeline() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
}

void GlBackend::ChunkPipeline::update(const Mesh& mesh) {
  num_verticies = mesh.vertices.size();
  model = mesh.model;
  glBindBuffer(GL_ARRAY_BUFFER, vbo); 
  glBufferData(GL_ARRAY_BUFFER, num_verticies * sizeof(Mesh::Vertex), mesh.vertices.data(), GL_STATIC_DRAW);
}

void GlBackend::ChunkPipeline::draw() const {
  glUseProgram(shader_program);
  glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, num_verticies);
  glUseProgram(0);
}

Result GlBackend::ChunkPipeline::create_shader_program() {
  std::filesystem::path resource_dir, vertex_shader_path, fragment_shader_path;
  Resources::get_resource_path(resource_dir);
  vertex_shader_path = resource_dir / Resources::SHADER_PATH / "chunk.vert.glsl";
  fragment_shader_path = resource_dir / Resources::SHADER_PATH / "chunk.frag.glsl";

  std::string vertex_shader_source, fragment_shader_source;
  load_shader_source(vertex_shader_source, vertex_shader_path);
  load_shader_source(fragment_shader_source, fragment_shader_path);

  GLuint vertex_shader, fragment_shader;
  
  Result vert_compile_res = compile_shader(vertex_shader, vertex_shader_source.c_str(), GL_VERTEX_SHADER);
  Result frag_compile_res = compile_shader(fragment_shader, fragment_shader_source.c_str(), GL_FRAGMENT_SHADER);

  if (vert_compile_res != Result::SUCCESS || frag_compile_res != Result::SUCCESS) {
    return Result::FAILURE_GLSL_SHADER_COMPILE;
  }

  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  GLint success;
  GLchar info_log[1024];
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 1024, NULL, info_log);
    LOG_ERROR("Failed to link chunk pipeline program");
    LOG_ERROR(info_log);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  model_location = glGetUniformLocation(shader_program, "model");
  view_location  = glGetUniformLocation(shader_program, "view");
  projection_location  = glGetUniformLocation(shader_program, "projection");

  return Result::SUCCESS;
}
