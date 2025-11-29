#include "scene.hpp"
#include <glad/glad.h>
#include <iostream>

Scene::Scene()
    : SCR_WIDTH(800), SCR_HEIGHT(800), camera_{glm::vec3(0.0f, 0.0f, 3.0f)},
      lastX(SCR_WIDTH / 2.0f), lastY(SCR_HEIGHT / 2.0f), firstMouse(true),
      deltaTime(0.0f), lastFrame(0.0f) {

  create_window();
  init_glad();
}

Scene::~Scene() {
  if (window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate(); // see note about global state below
}

bool Scene::shouldClose() const { return glfwWindowShouldClose(window); }

void Scene::swapBuffers() { glfwSwapBuffers(window); }

void Scene::processInput() {
  float currentFrame = static_cast<float>(glfwGetTime());
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(RIGHT, deltaTime);
}

GLFWwindow *Scene::create_window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    std::exit(-1);
  }
  glfwMakeContextCurrent(window);

  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return window;
}

int Scene::init_glad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    std::exit(-1);
  }
  return 0;
}

void Scene::framebuffer_size_callback(GLFWwindow *win, int width, int height) {
  auto *self = static_cast<Scene *>(glfwGetWindowUserPointer(win));
  if (!self)
    return;
  self->SCR_WIDTH = width;
  self->SCR_HEIGHT = height;
  glViewport(0, 0, width, height);
}

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
  }

  float xoffset = xpos - self->lastX;
  float yoffset = self->lastY - ypos;

  self->lastX = xpos;
  self->lastY = ypos;

  self->camera_.ProcessMouseMovement(xoffset, yoffset);
}
