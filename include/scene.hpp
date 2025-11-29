#pragma once
#include "camera.hpp"
#include <GLFW/glfw3.h>

typedef unsigned int uint;

class Scene {
public:
  Scene();
  ~Scene();

  GLFWwindow *get_window() const { return window; }
  uint width() const { return SCR_WIDTH; }
  uint height() const { return SCR_HEIGHT; }

  Camera &camera() { return camera_; }
  const Camera &camera() const { return camera_; }

  bool shouldClose() const; // or isOpen()
  void swapBuffers();
  void processInput();

private:
  GLFWwindow *window = nullptr;
  uint SCR_WIDTH;
  uint SCR_HEIGHT;
  Camera camera_;

  float lastX;
  float lastY;
  bool firstMouse = true;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  GLFWwindow *create_window();
  int init_glad();

  static void framebuffer_size_callback(GLFWwindow *win, int width, int height);
  static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
};
