#pragma once
#include <glm/glm.hpp>

#include "material/MyMaterial.h"

/**
 * Material for "Improved Model" (Whitted 1980)
 */
class Material : public MyMaterial {
 public:
  float Kd = 0.5f;   // diffuse reflection constant
  float Ks = 0.25f;  // the specular reflection coefficient
  float Kt = 0.25f;  // the transmission coefficient
  float n = 8.0f;  // an exponent that depends on the glossiness of the surface
};
