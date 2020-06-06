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
 * Diffuse : uniform sampling
 */
class DiffuseMaterial : public MaterialBase {
  float Kd = 0.5f;

 public:
  virtual glm::vec3 scatter(const glm::vec3& normal) override {
    // uniform sampling the hemisphere
    glm::vec3 rand = glm::sphericalRand(1.0f);
    rand.z = fabsf(rand.z);
    ONB onb(normal);
    return glm::normalize(onb.localToWorld(rand));
  }

  virtual float pdf(const glm::vec3& dir, const glm::vec3& normal) override {
    // uniform sampling the hemisphere: pdf = 1 / (2 * PI)
    constexpr float PI = glm::pi<float>();

    float cosine = glm::dot(dir, normal);
    return (cosine > 0) ? 0.5f / PI : 0.f;
  }

  virtual float evaluate(const glm::vec3& wo, const glm::vec3& wi,
                         const glm::vec3& normal) override {
    float cosine = glm::dot(normal, wi);
    if (cosine > 0)
      return Kd / glm::pi<float>();
    else
      return 0;
  }
};

}  // namespace RayTracingHistory
