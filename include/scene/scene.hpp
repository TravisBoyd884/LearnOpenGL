#pragma once

#include "core/window.hpp"
#include "render/camera.hpp"

typedef unsigned int uint;

class Scene {
public:
  explicit Scene(Window &window);
  ~Scene();

  Camera &camera() { return camera_; }
  const Camera &camera() const { return camera_; }

  void processInput();
  static void framebuffer_size_callback(GLFWwindow *win, int width, int height);
  static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

private:
  Window &window_;

  uint SCR_WIDTH;
  uint SCR_HEIGHT;
  Camera camera_;

  float lastX;
  float lastY;
  bool firstMouse = true;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
};
