/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "MyTexture.h"

namespace RTKit2 {

class CheckerTexture : public MyTexture {
  glm::vec3 mColorOdd = glm::vec3(0.8f, 0, 0);
  glm::vec3 mColorEven = glm::vec3(0.8f, 0.8f, 0.f);
  float mP = 2.f;

 public:
  virtual glm::vec3 sample(const glm::vec2& uv, const glm::vec3& p) override {
    float sines = sinf(p.x * mP) * cosf(p.z * mP);
    return sines < 0 ? mColorOdd : mColorEven;
  }

  void setColor(const glm::vec3& colorOdd, const glm::vec3& colorEven){
    mColorOdd = colorOdd;
    mColorEven = colorEven;
  }
};
}  // namespace RTKit1
