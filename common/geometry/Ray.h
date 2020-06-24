/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>

namespace RayTracingHistory {

/*
 * p(t) = origin + t*direction
 */
struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;
  float time = 0.0f;

  Ray() : origin(0, 0, 0), direction(0, 1, 0) {}

  Ray(const glm::vec3& inOrigin, const glm::vec3& inDir, bool bUnitDir = true) {
    origin = inOrigin;
    direction = inDir;
    if (bUnitDir) direction = glm::normalize(inDir);
  }

  glm::vec3 getPoint(float t) const { return origin + direction * t; }

  void applayBiasOffset(const glm::vec3& N, float maxBias,
                        float minBias = 0.0001f) {
    float a = glm::max(0.0f, glm::dot(N, glm::normalize(direction)));
    float bias = glm::mix(maxBias, minBias, a);
    origin += N * bias;
  }
};
}  // namespace RayTracingHistory
