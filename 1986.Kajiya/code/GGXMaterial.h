/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

#include "MaterialBase.h"
#include "geometry/ONB.h"

namespace RayTracingHistory {

// Microfacet specular = D*G*F / (4*NoL*NoV) = D*Vis*F
// Vis = G / (4*NoL*NoV)

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float F_schlick() { return 1; }

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough
// surfaces"]
float D_GGX() { return 1; }

/**
 * GGX Microsfaced Model
 */
class GGXMaterial : public MaterialBase {
  float Kd = 1.0f;

  float roughness = 0.5f;

 public:
  virtual float evaluate(const glm::vec3& wi, const glm::vec3& wo,
                         const glm::vec3& normal) const override {
    return Kd / glm::pi<float>();
  }

  virtual glm::vec3 sample(const glm::vec3& wo,
                           const glm::vec3& normal) const override {
    // uniform sampling the upper hemisphere
    // rejection sampling
    glm::vec3 rand = glm::sphericalRand(1.0f);
    float t = glm::length(rand);
    if (glm::dot(rand, normal) > 0)
      return rand;
    else
      return -rand;
  }

  virtual float pdf(const glm::vec3& wi, const glm::vec3& normal) {
    // area of the hemishpere
    if (glm::dot(wi, normal) > 0.0f)
      return 1.0f / (2.0f * glm::pi<float>());
    else
      return 0.0f;
  }
};

}  // namespace RayTracingHistory
