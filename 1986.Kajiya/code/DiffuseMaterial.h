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
  float Kd = 0.5f;

 public:
  virtual float BRDF(const glm::vec3& wi, const glm::vec3& w0) const override {
    return Kd / glm::pi<float>();
  }

  virtual glm::vec3 scatter(const glm::vec3& wo,
                            const glm::vec3& normal) const override {
    // uniform sampling the hemisphere
    glm::vec3 rand = glm::sphericalRand(1.0f);
    rand.z = fabsf(rand.z);
    ONB onb(normal);
    return glm::normalize(onb.localToWorld(rand));
  }
};

}  // namespace RayTracingHistory
