#pragma once
#include <glm/glm.hpp>

class MyTexture {
 public:
  MyTexture() = default;
  virtual ~MyTexture() = default;

  virtual glm::vec3 sample(const glm::vec2& uv, const glm::vec3& p) = 0;
};

class ConstantTexture : public MyTexture {
  glm::vec3 mColor = glm::vec3(1);

 public:
  void setColor(const glm::vec3& color) { mColor = color; }

  virtual glm::vec3 sample(const glm::vec2& uv, const glm::vec3& p) override { return mColor; }
};
