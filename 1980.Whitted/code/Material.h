
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

namespace RayTracingHistory {

/**
 * Material for "Improved Model" (Whitted 1980)
 */
class Material : public MyMaterial {
  std::shared_ptr<MyTexture> albedo;

 public:
  Material() {}

  Material& setColor(const glm::vec3& color) {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(color);
    albedo = ct;
    return *this;
  }

  Material& setCheckerTexture() {
    albedo = std::make_shared<CheckerTexture>();
    return *this;
  }

  Material& setParam(float inKd, float inKs, float inKt, float inN,
                     float inKn) {
    Kd = inKd;
    Ks = inKs;
    Kt = inKt;
    n = inN;
    Kn = inKn;
    return *this;
  }

  glm::vec3 sampleAlbedo(const glm::vec2& uv, const glm::vec3& p) {
    if (albedo)
      return albedo->sample(uv, p);
    else
      return glm::vec3(1);
  }

 public:
  float Kd = 0.5f;  // diffuse reflection constant
  float Ks = 0.4f;  // the specular reflection coefficient
  float Kt = 0.1f;  // the transmission coefficient
  float n = 99;     // an exponent that depends on the glossiness of the surface
  float Kn = 1.34f;  // the index of refraction
};

}  // namespace RayTracingHistory
