#include "core.h"
#include "render/gl.h"

#include "update/ecs/ecs.h"
#include "utils/resources.h"

#include <filesystem>
#include <string>
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

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

  Result chunk_pipeline_prepare_res = gl_backend->prepare_chunk_pipeline();
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

Result GlBackend::draw(const ECS* const ecs) {
  glClear(GL_COLOR_BUFFER_BIT);

  draw_chunk_components(ecs);

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

Result GlBackend::prepare_chunk_pipeline() {
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

  chunk_pipeline.shader_program = glCreateProgram();
  glAttachShader(chunk_pipeline.shader_program, vertex_shader);
  glAttachShader(chunk_pipeline.shader_program, fragment_shader);
  glLinkProgram(chunk_pipeline.shader_program);

  GLint success;
  GLchar info_log[1024];
  glGetProgramiv(chunk_pipeline.shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(chunk_pipeline.shader_program, 1024, NULL, info_log);
    LOG_ERROR("Failed to link chunk pipeline program");
    LOG_ERROR(info_log);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return Result::SUCCESS;
}

void GlBackend::draw_chunk_components(const ECS* const ecs) {
  glUseProgram(chunk_pipeline.shader_program);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glUseProgram(0);
}
