#include "core/window.hpp"
#include "pch.hpp"
#include "render/mesh.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"
#include "scene/scene.hpp"
#include <GLFW/glfw3.h>

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// + 4 makes FPS closer to target idk why
const double TARGET_FPS = 144.0 + 4;
const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS; // seconds

void limitFrameRate(
    const std::chrono::high_resolution_clock::time_point &start) {
  auto frameEnd = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> frameDuration = frameEnd - start;
  double frameTime = frameDuration.count();

  if (frameTime < TARGET_FRAME_TIME) {
    double sleepTime = TARGET_FRAME_TIME - frameTime;
    std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
  }
}

static void glfw_error_callback(int error, const char *description) {
  std::cerr << "[GLFW ERROR] (" << error << "): " << description << '\n';
}

int main() {
  glfwSetErrorCallback(glfw_error_callback);
  Window window(800, 800, "LearnOpenGL");
  Scene scene(window);

  glEnable(GL_DEPTH_TEST);

  Shader myShader("../assets/shaders/shader.vert",
                  "../assets/shaders/shader.frag");

  Shader lightShader("../assets/shaders/light.vert",
                     "../assets/shaders/light.frag");
  Shader cubeShader("../assets/shaders/cube.vert",
                    "../assets/shaders/cube.frag");

  Mesh cubeMesh = Mesh::createCube();

  // load textures (we now use a utility function to keep the code more
  // organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMap = loadTexture("../assets/textures/container2.png");
  unsigned int specularMap =
      loadTexture("../assets/textures/container2_specular.png");

  // shader configuration
  // --------------------
  cubeShader.use();
  cubeShader.setInt("material.diffuse", 0);
  cubeShader.setInt("material.specular", 1);

  double previousTime = glfwGetTime();
  int frameCount = 0;

  while (!window.shouldClose()) {
    auto frameStart = std::chrono::high_resolution_clock::now();
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed, compute FPS
    if (currentTime - previousTime >= 1.0) {
      double fps = frameCount / (currentTime - previousTime);
      std::cout << "FPS: " << fps << "\n";

      frameCount = 0;
      previousTime = currentTime;
    }

    scene.processInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    cubeShader.use();
    cubeShader.setVec3("light.position", lightPos);
    cubeShader.setVec3("viewPos", scene.camera().Position);

    // light properties
    cubeShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    cubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    cubeShader.setFloat("material.shininess", 64.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(scene.camera().Zoom),
        (float)window.width() / (float)window.height(), 0.1f, 100.0f);
    glm::mat4 view = scene.camera().GetViewMatrix();
    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    cubeShader.setMat4("model", model);
    cubeShader.setMat4("modelNormal", glm::transpose(glm::inverse(model)));

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // render the cube
    glBindVertexArray(cubeMesh.vao());
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // also draw the lamp object
    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightShader.setMat4("model", model);

    glBindVertexArray(cubeMesh.vao());
    glDrawArrays(GL_TRIANGLES, 0, 36);

    window.swapBuffers();
    glfwPollEvents();
    // frame end time
    auto frameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> frameDuration = frameEnd - frameStart;
    double frameTime = frameDuration.count(); // in seconds

    // if the frame was faster than our target, sleep the remainder
    if (frameTime < TARGET_FRAME_TIME) {
      double sleepTime = TARGET_FRAME_TIME - frameTime;

      std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
    }
  }

  return 0;
}
