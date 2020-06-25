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
  virtual float evaluate(const glm::vec3& wi, const glm::vec3& wo,
                         const glm::vec3& normal) const override {
    return Kd / glm::pi<float>();
  }

  virtual SampleResult sample(const glm::vec3& wo,
                              const glm::vec3& normal) const override {
    // uniform sampling the upper hemisphere
    // rejection sampling
    glm::vec3 rand = glm::sphericalRand(1.0f);
    float t = glm::length(rand);

    SampleResult ret;
    ret.pdf = 1.0f / (2.0f * glm::pi<float>());
    if (glm::dot(rand, normal) > 0)
      ret.scattered = rand;
    else
      ret.scattered = -rand;
    return ret;
  }
};

}  // namespace RayTracingHistory
