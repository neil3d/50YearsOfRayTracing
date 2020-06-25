/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>

#include "material/CheckerTexture.h"
#include "material/MyMaterial.h"
#include "material/Texture2D.h"

namespace RayTracingHistory {

struct SampleResult {
  glm::vec3 scattered;
  float pdf;
};

class MaterialBase : public MyMaterial {
 public:
  glm::vec3 getBaseColor(const glm::vec2& uv, const glm::vec3& p) {
    if (mBaseColor)
      return mBaseColor->sample(uv, p);
    else
      return glm::vec3(1);
  }

  MaterialBase& setColor(const glm::vec3& color) {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(color);
    mBaseColor = ct;
    return *this;
  }

  MaterialBase& enableLight() {
    mLight = true;
    return *this;
  }

  bool isLight() const { return mLight; }

  // for a given incident, outgoing direction, evaluate the corresponding BRDF
  // value
  virtual float evaluate(const glm::vec3& wi, const glm::vec3& wo,
                         const glm::vec3& normal) const = 0;

  // given outgoing direction, sample a incident direction to continue the path
  virtual SampleResult sample(const glm::vec3& wo,
                           const glm::vec3& normal) const = 0;

 protected:
  std::shared_ptr<MyTexture> mBaseColor;
  bool mLight = false;  // for light shape rendering
};

}  // namespace RayTracingHistory
