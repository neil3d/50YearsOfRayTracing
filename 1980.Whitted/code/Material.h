#pragma once
#include <glm/glm.hpp>

#include "material/CheckerTexture.h"
#include "material/MyMaterial.h"

/**
 * Material for "Improved Model" (Whitted 1980)
 */
class Material : public MyMaterial {
  std::shared_ptr<MyTexture> albedo;

 public:
  Material() {}

  void setColor(const glm::vec3& color) {
    auto ct = std::make_shared<ConstantTexture>();
    ct->setColor(color);
    albedo = ct;
  }
  void setCheckerTexture() { albedo = std::make_shared<CheckerTexture>(); }

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
  float n =
      100.0f;  // an exponent that depends on the glossiness of the surface
};
