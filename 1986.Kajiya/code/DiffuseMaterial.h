/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/random.hpp>

#include "MaterialBase.h"
#include "geometry/ONB.h"

namespace RayTracingHistory {

/**
 * Diffuse : uniform sampling
 */
class DiffuseMaterial : public MaterialBase {
 public:
  virtual glm::vec3 scatter(const glm::vec3& normal) override {
    // uniform sampling the hemisphere
    glm::vec3 rand = glm::sphericalRand(1.0f);
    rand.z = fabsf(rand.z);
    ONB onb(normal);
    return onb.localToWorld(rand);
  }

  virtual float pdf(const glm::vec3& dir, const glm::vec3& normal) override {
    // uniform sampling the hemisphere: pdf = 1 / (2 * PI)
    float cosine = glm::dot(dir, normal);
    return (cosine > 0) ? 0.5f / M_PI : 1.f;
  }
};

}  // namespace RayTracingHistory
