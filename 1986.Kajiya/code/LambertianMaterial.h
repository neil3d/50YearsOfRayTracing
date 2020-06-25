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
 * Cosine Importance Sampling
 */
class LambertianMaterial : public MaterialBase {
  float Kd = 1.0f;

 public:
  virtual float evaluate(const glm::vec3& wi, const glm::vec3& wo,
                         const glm::vec3& normal) const override {
    float d = glm::dot(wi, normal);
    return Kd * glm::max(0.0f, d);
  }

  virtual glm::vec3 sample(const glm::vec3& wo,
                           const glm::vec3& normal) const override {
    float r1 = glm::linearRand(0.0f, 1.0f);
    float r2 = glm::linearRand(0.0f, 1.0f);

    float z = sqrt(1 - r2);
    float phi = 2 * glm::pi<float>() * r1;
    float x = cos(phi) * sqrt(r2);
    float y = sin(phi) * sqrt(r2);

    glm::vec3 localSample(x, y, z);
    ONB onb(normal);
    return onb.localToWorld(localSample);
  }

  virtual float pdf(const glm::vec3& wi, const glm::vec3& normal) {
    // area of the hemishpere
    float d = glm::dot(wi, normal);
    if (d > 0.0f)
      return d / glm::pi<float>();
    else
      return 0.0f;
  }
};

}  // namespace RayTracingHistory
