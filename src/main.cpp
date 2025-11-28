#include "scene.hpp"
#include <glad/glad.h>

#include "camera.hpp"
#include "shader.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
  Scene s;

  glEnable(GL_DEPTH_TEST);

  Shader myShader("../shaders/shader.vert", "../shaders/shader.frag");

  Shader lightShader("../shaders/light.vert", "../shaders/light.frag");
  Shader cubeShader("../shaders/cube.vert", "../shaders/cube.frag");

  // clang-format off
float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

  // clang-format on

  // first, configure the cube's VAO (and VBO)
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // second, configure the light's VAO (VBO stays the same; the vertices are the
  // same for the light object which is also a 3D cube)
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // we only need to bind to the VBO (to link it with glVertexAttribPointer), no
  // need to fill it; the VBO's data already contains all we need (it's already
  // bound, but we do it again for educational purposes)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (s.shouldClose()) {

    s.processInput();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    cubeShader.use();
    cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(s.get_camera().Zoom),
                         (float)s.width() / (float)s.height(), 0.1f, 100.0f);
    glm::mat4 view = s.get_camera().GetViewMatrix();
    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    cubeShader.setMat4("model", model);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // also draw the lamp object
    lightShader.use();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightShader.setMat4("model", model);
    lightShader.setVec3("aColor",
                        glm::vec3(sin((float)glfwGetTime()), 1.0f, 1.0f));

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    s.swapBuffers();
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
