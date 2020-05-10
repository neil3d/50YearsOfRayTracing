#pragma once
#include <glm/glm.hpp>

#include "material/MyMaterial.h"

/**
 * Material for "Improved Model" (Whitted 1980)
 */
class Material : public MyMaterial {
  std::shared_ptr<ConstantTexture> albedo;

 public:
  Material() { albedo = std::make_shared<ConstantTexture>(); }

  void setColor(const glm::vec3& color) { albedo->setColor(color); }

  glm::vec3 sampleAlbedo(float u, float v) { return albedo->sample(u, v); }

 public:
  float Kd = 0.5f;  // diffuse reflection constant
  float Ks = 0.4f;  // the specular reflection coefficient
  float Kt = 0.1f;  // the transmission coefficient
  float n = 100.0f;   // an exponent that depends on the glossiness of the surface
};
