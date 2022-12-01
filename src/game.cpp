#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>

using std::vector;
using std::runtime_error;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ostringstream;
using std::to_string;

// resize window on drag
void
framebuffer_size_callback(GLFWwindow *window, const int w, const int h) {
  cerr << "window has been resized!" << endl;
  glViewport(0, 0, w, h);
}

void
process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void
read_file_to_string(const string &fn, const size_t array_size, char *str) {
  ostringstream oss;
  ifstream in(fn);
  if (!in.good())
    throw runtime_error("cannot open file " + fn);

  oss << in.rdbuf();

  if (oss.str().size() > array_size)
    throw runtime_error("file size exceeds " + to_string(array_size) + ": " + fn);

  strcpy(str, oss.str().c_str());
}

enum shader_type {TYPE_VERTEX_SHADER, TYPE_FRAGMENT_SHADER};
template<const shader_type type>
GLuint
compile_shader() {
  // init shader
  static const size_t MAX_FILE_SIZE = 65536;
  char *shader_source = (char*)malloc(MAX_FILE_SIZE*sizeof(char));

  GLuint shader;
  if (type == TYPE_VERTEX_SHADER) {
    read_file_to_string("shaders/vertex.shader", MAX_FILE_SIZE, shader_source);
    shader = glCreateShader(GL_VERTEX_SHADER);
  }
  else {
    read_file_to_string("shaders/fragment.shader", MAX_FILE_SIZE, shader_source);
    shader = glCreateShader(GL_FRAGMENT_SHADER);
  }

  glShaderSource(shader, 1, &shader_source, NULL);
  glCompileShader(shader);

  // check if init was OK
  int success;
  char info_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    const bool vtx = (type == TYPE_VERTEX_SHADER);
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    cerr << "problem with " << (vtx ? "vertex" : "fragment")
         << " shader: " << info_log;
    return 0;
  }
  return shader;
}

static GLuint
init_shaders() {
  GLuint vertex_shader = compile_shader<TYPE_VERTEX_SHADER>();
  GLuint fragment_shader = compile_shader<TYPE_FRAGMENT_SHADER>();

  if (vertex_shader == 0 || fragment_shader == 0)
    return false;

  GLuint shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  // check if init was OK
  int success;
  char info_log[512];
  glGetShaderiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    cerr << "problem with linking shader to program: " << info_log << endl;
    return 0;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return shader_program;
}

inline void
glClearColor255(const int r, const int g, const int b, const int a) {
  glClearColor(
      static_cast<float>(r)/255.0,
      static_cast<float>(g)/255.0,
      static_cast<float>(b)/255.0,
      static_cast<float>(a)/255.0
  );
}

inline void
init_vertex_buffer(GLuint &vertex_array_object,
                   GLuint &vertex_buffer_object,
                   GLuint &element_buffer_object) {
  // generate
  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  // bind
  glBindVertexArray(vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
}

void
load_vertices(const vector<float> &vertices,
              const vector<uint32_t> &indices) {
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

inline void
postprocess_vertex_buffer() {
  // coordinates attribute (location = 0 on vertex.shader)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);

  // color attribute (location = 1 on vertex.shader)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*) (3*sizeof(float)));
  glEnableVertexAttribArray(1);
}

int
main(int argc, const char **argv) {
  static const size_t SCREEN_WIDTH = 1024;
  static const size_t SCREEN_HEIGHT = 768;
  static const string GAME_NAME = "First Game";

  const bool wireframe_mode = (argc > 1 && strcmp(argv[1], "wireframe") == 0);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(
    SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME.c_str(),
    NULL, NULL
  );

  // init window
  if (window == NULL) {
    glfwTerminate();
    throw runtime_error("Failed to initialize window with GLFW!");
  }

  // bind resizing to drag operation
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwMakeContextCurrent(window);

  // init GLAD
  if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
    glfwTerminate();
    throw runtime_error("Failed to initialize GLAD!");
  }

  // shader program
  GLuint shader_program = init_shaders();
  if (!shader_program) {
    glfwTerminate();
    throw runtime_error("Failed to compile shaders!");
  }

  GLuint vertex_array_object = 0;
  GLuint vertex_buffer_object = 0;
  GLuint element_buffer_object = 0;
  init_vertex_buffer(vertex_array_object,
                     vertex_buffer_object,
                     element_buffer_object);

  if (vertex_array_object == 0 ||
      vertex_buffer_object == 0 ||
      element_buffer_object == 0) {
    glfwTerminate();
    throw runtime_error("Failed to init vertex buffer");
  }

  // triangle
  vector<float> square = {
    -0.5f, -0.5f, 0.f, 0.5f, 0.f, 0.f,
     0.5f, -0.5f, 0.f, 0.5f, 0.5f, 0.5f,
    -0.5f,  0.5f, 0.f, 0.5f, 0.5f, 0.f,
     0.5f,  0.5f, 0.5f, 0.5f, 0.5f, 0.5f
  };

  vector<uint32_t> square_indices = {
    0, 1, 2,
    1, 2, 3
  };

  load_vertices(square, square_indices);
  postprocess_vertex_buffer();


  if (wireframe_mode) {
    cerr << "running in wireframe mode" << endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  glUseProgram(shader_program);
  while (!glfwWindowShouldClose(window)) {
    // inputs
    process_input(window);

    // render
    glClearColor255(42, 94, 140, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw
    glBindVertexArray(vertex_array_object);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // post
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);
  glDeleteProgram(shader_program);
  glfwTerminate();
  std::cerr << "Bye!" << endl;
  return EXIT_SUCCESS;
}

