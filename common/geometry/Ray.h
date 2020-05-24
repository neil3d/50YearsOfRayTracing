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

  Ray() : origin(0, 0, 0), direction(0, 1, 0) {}

  Ray(const glm::vec3& inOrigin, const glm::vec3& inDir) {
    origin = inOrigin;
    direction = glm::normalize(inDir);
  }

  glm::vec3 getPoint(float t) const { return origin + direction * t; }
};
}  // namespace RayTracingHistory
