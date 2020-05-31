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

class Material : public MyMaterial {
  std::shared_ptr<MyTexture> albedo;

 public:
  float gloss = 0.0f;
  float Kr = 0.4f;  // the reflection coefficient
  glm::vec2 uvTiling = {1, 1};

  Material& setGloss(float g) {
    gloss = g;
    return *this;
  }

  Material& setCoefficient(float _Kr) {
    Kr = _Kr;
    return *this;
  }

  Material& setTiling(float u, float v) {
    uvTiling = glm::vec2(u, v);
    return *this;
  }

  Material& setTexture2D(const std::string& szFile) {
    auto tex = std::make_shared<Texture2D>();
    tex->loadImage(szFile);
    albedo = tex;
    return *this;
  }

  Material& setColor(const glm::vec3& color) {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(color);
    albedo = ct;
    return *this;
  }

  Material& setCheckerTexture(const glm::vec3& colorOdd,
                              const glm::vec3& colorEven) {
    auto checker = std::make_shared<CheckerTexture>();
    checker->setColor(colorOdd, colorEven);
    albedo = checker;
    return *this;
  }

  glm::vec3 sampleAlbedo(const glm::vec2& uv, const glm::vec3& p) {
    if (albedo)
      return albedo->sample(uvTiling * uv, p);
    else
      return glm::vec3(1);
  }
};

}  // namespace RayTracingHistory
