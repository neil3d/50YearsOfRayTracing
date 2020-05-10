#pragma once
#include <glm/glm.hpp>

#include "material/MyMaterial.h"

/**
 * Material for "Improved Model" (Whitted 1980)
 */
class Material : public MyMaterial {
 public:
  glm::vec3 Kd;    // diffuse reflection constant
  glm::vec3 Ks;    // the specular reflection coefficient
  glm::vec3 Kt;    // the transmission coefficient
  float n = 8.0f;  // an exponent that depends on the glossiness of the surface
};
