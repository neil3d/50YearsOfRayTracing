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

inline glm::vec3 _randomCosineDirection() {
  glm::vec2 rand = glm::diskRand(0.5f);

  auto r1 = rand.x + 0.5f;
  auto r2 = rand.y + 0.5f;
  auto z = sqrt(1 - r2);

  auto phi = 2 * M_PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);

  return glm::vec3(x, y, z);
}

/**
 * For Lambertian BRDF (ρ = R/π) use a cosine density function
 */
class LambertianMaterial : public MaterialBase {
 public:
  virtual glm::vec3 scatter(const glm::vec3& normal) override {
    ONB onb(normal);
    glm::vec3 rand = _randomCosineDirection();
    return onb.localToWorld(rand);
  }

  virtual float pdf(const glm::vec3& dir, const glm::vec3& normal) override {
    float cosine = glm::dot(dir, normal);
    return (cosine <= 0) ? 0 : cosine / M_PI;
  }
};

}  // namespace RayTracingHistory
