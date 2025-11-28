#pragma once
#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

typedef unsigned int uint;

class Scene {
public:
  Scene()
      : SCR_WIDTH(800), SCR_HEIGHT(800), camera{glm::vec3(0.0f, 0.0f, 3.0f)},
        lastX(SCR_WIDTH / 2.0f), lastY(SCR_HEIGHT / 2.0f), firstMouse(true),
        deltaTime(0.0f), lastFrame(0.0f) {
    create_window();
    init_glad();
  }
  GLFWwindow *get_window() const { return window; }
  uint width() const { return SCR_WIDTH; }
  uint height() const { return SCR_HEIGHT; }
  Camera get_camera() const { return camera; };
  bool shouldClose() { return !glfwWindowShouldClose(window); }
  void swapBuffers() { glfwSwapBuffers(window); }
  void processInput() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.ProcessKeyboard(RIGHT, deltaTime);
  }

  ~Scene() {
    if (window) {
      glfwDestroyWindow(window);
    }
    glfwTerminate();
  }

private:
  GLFWwindow *window;
  uint SCR_WIDTH;
  uint SCR_HEIGHT;
  Camera camera;
  float lastX;
  float lastY;
  bool firstMouse = true;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  GLFWwindow *create_window() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      std::exit(-1);
    }
    glfwMakeContextCurrent(window);

    // attach "this" to the GLFWwindow so we can get it in the static callback
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
  }
  int init_glad() {

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      std::exit(-1);
    }
    return 0;
  }
  // static wrapper with the signature GLFW expects
  static void framebuffer_size_callback(GLFWwindow *win, int width,
                                        int height) {
    // recover the Window instance
    auto *self = static_cast<Scene *>(glfwGetWindowUserPointer(win));
    if (!self)
      return;

    self->SCR_WIDTH = width;
    self->SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
  }

  static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
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
    float yoffset = self->lastY -
                    ypos; // reversed since y-coordinates go from bottom to top

    self->lastX = xpos;
    self->lastY = ypos;

    self->camera.ProcessMouseMovement(xoffset, yoffset);
  }
};
