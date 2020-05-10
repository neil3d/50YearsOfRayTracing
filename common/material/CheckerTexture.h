#pragma once
#include "MyTexture.h"

class CheckerTexture : public MyTexture {
  glm::vec3 mColorOdd = glm::vec3(0.8f, 0, 0);
  glm::vec3 mColorEven = glm::vec3(0.8f, 0.8f, 0.f);
  float mP = 2.f;

 public:
  virtual glm::vec3 sample(const glm::vec2& uv, const glm::vec3& p) override {
    float sines = sinf(p.x * mP) * cosf(p.z * mP);
    return sines < 0 ? mColorOdd : mColorEven;
  }
};