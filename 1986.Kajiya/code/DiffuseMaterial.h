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

/**
 * diffuse : uniform sampling
 */
class DiffuseMaterial : public MaterialBase {
  float Kd = 1.0f;

 public:
  virtual float evaluate(const glm::vec3& wi,
                         const glm::vec3& wo) const override {
    return Kd / glm::pi<float>();
  }

  virtual glm::vec3 sample(const glm::vec3& wo,
                           const glm::vec3& normal) const override {
    // uniform sampling the upper hemisphere
    glm::vec3 rand = glm::sphericalRand(1.0f);
    rand.z = fabsf(rand.z);
    ONB onb(normal);
    return glm::normalize(onb.localToWorld(rand));
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
