#include "scene/scene.hpp"
#include <glad/glad.h>
#include <iostream>
Scene::Scene(Window &window)
    : window_(window), SCR_WIDTH(window.width()), SCR_HEIGHT(window.height()),
      camera_{glm::vec3(0.0f, 0.0f, 3.0f)}, lastX(SCR_WIDTH / 2.0f),
      lastY(SCR_HEIGHT / 2.0f), firstMouse(true), deltaTime(0.0f),
      lastFrame(0.0f) {

  GLFWwindow *win = window_.get_window();

  glfwSetWindowUserPointer(win, this);
  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
  glfwSetCursorPosCallback(win, mouse_callback);
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Scene::~Scene() {}

void Scene::processInput() {
  GLFWwindow *win = window_.get_window();

  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(win, true);

  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(RIGHT, deltaTime);
}

void Scene::framebuffer_size_callback(GLFWwindow *win, int width, int height) {
  auto *self = static_cast<Scene *>(glfwGetWindowUserPointer(win));
  if (!self)
    return;

  self->SCR_WIDTH = width;
  self->SCR_HEIGHT = height;
  self->window_.setDimensions(width, height);

  glViewport(0, 0, width, height);
}
//
void Scene::mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  auto *self = static_cast<Scene *>(glfwGetWindowUserPointer(window));
  if (!self)
    return;

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (self->firstMouse) {
    self->lastX = xpos;
    self->lastY = ypos;
    self->firstMouse = false;
    return; // <--- don't compute offsets on this frame
  }

  float xoffset = xpos - self->lastX;
  float yoffset =
      self->lastY - ypos; // reversed since y-coordinates go from top to bottom

  self->lastX = xpos;
  self->lastY = ypos;

  self->camera_.ProcessMouseMovement(xoffset, yoffset);
}
