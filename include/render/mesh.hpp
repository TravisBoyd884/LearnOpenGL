#pragma once

#include <glad/glad.h>

class Mesh {
public:
  Mesh() = default;
  Mesh(GLuint vao, GLuint vbo, GLsizei vertexCount);

  // non-copyable (owning GL resources)
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  // movable
  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(Mesh &&other) noexcept;

  ~Mesh();

  void draw() const;

  GLuint vao() const { return vao_; }
  GLuint vbo() const { return vbo_; }
  GLsizei vertexCount() const { return vertexCount_; }

  // Our cube factory
  static Mesh createCube();

private:
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLsizei vertexCount_ = 0;
};
