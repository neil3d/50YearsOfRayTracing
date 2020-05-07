#pragma once
#include <glm/glm.hpp>

class MyTexture {
 public:
  virtual glm::vec3 sample(float u, float v) = 0;
};
