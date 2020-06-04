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

class MaterialBase : public MyMaterial {
 public:
  MaterialBase() {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(glm::vec3(1, 0.5f, 0.25f));
    mBaseColor = ct;
  }

  glm::vec3 getBaseColor(const glm::vec2& uv, const glm::vec3& p) {
    if (mBaseColor)
      return mBaseColor->sample(uv, p);
    else
      return glm::vec3(1);
  }

 protected:
  std::shared_ptr<MyTexture> mBaseColor;
  float mEmission = 0;
};

}  // namespace RayTracingHistory
