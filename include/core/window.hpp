#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
  Window(int width, int height, const std::string &title);
  ~Window();

  GLFWwindow *get_window() const { return window_; }
  bool shouldClose() const; // or isOpen()
  void swapBuffers();

  uint width() const { return SCR_WIDTH; }
  uint height() const { return SCR_HEIGHT; }

private:
  bool s_glfwInitialized;
  uint SCR_WIDTH;
  uint SCR_HEIGHT;
  GLFWwindow *window_ = nullptr;

  void init_GLFW();
  GLFWwindow *create_window(const std::string &title);
  int init_glad();
};
