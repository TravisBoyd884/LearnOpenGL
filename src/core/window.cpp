#include "core/window.hpp"
#include "pch.hpp"
#include <iostream>

Window::Window(int width, int height, const std::string &title)
    : SCR_WIDTH(800), SCR_HEIGHT(800) {

  init_GLFW();
  create_window(title);
  init_glad();
}

Window::~Window() {
  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
  }
  glfwTerminate();
}

void Window::init_GLFW() {
  if (s_glfwInitialized)
    return;

  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW\n";
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  s_glfwInitialized = true;
}

GLFWwindow *Window::create_window(const std::string &title) {
  window_ = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);
  if (!window_) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    std::exit(-1);
  }
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(0);

  return window_;
}

int Window::init_glad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    std::exit(-1);
  }
  return 0;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(window_); }

void Window::swapBuffers() { glfwSwapBuffers(window_); }
