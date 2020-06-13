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
  glm::vec3 getBaseColor(const glm::vec2& uv, const glm::vec3& p) {
    if (mBaseColor)
      return mBaseColor->sample(uv, p);
    else
      return glm::vec3(1);
  }

  MaterialBase& setEmission(float val) {
    mEmission = val;
    return *this;
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
  float getEmission() const { return mEmission; }

  virtual float BRDF(const glm::vec3& wi, const glm::vec3& w0) = 0;

 protected:
  std::shared_ptr<MyTexture> mBaseColor;
  float mEmission = 0;
  bool mLight = false;  // for light shape rendering
};

}  // namespace RayTracingHistory
