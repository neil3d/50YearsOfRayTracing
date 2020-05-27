/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "geometry/Ray.h"

namespace RayTracingHistory {

struct AreaLight {
  glm::vec3 edge1, edge2;
  glm::vec3 corner;

  float ambient = 0.1f;
  float intensity = 2.0f;

  Ray jitteredShadowRay(const glm::vec3& shadingPt, const glm::vec2& xi) const {
    glm::vec3 pos = corner + xi.x * edge1 + xi.y * edge2;
    glm::vec3 L = glm::normalize(pos - shadingPt);
    return Ray(shadingPt, L);
  }

  AreaLight& setParallelogram(const glm::vec3& _edge1, const glm::vec3& _edge2,
                              const glm::vec3& _corner) {
    edge1 = _edge1;
    edge2 = _edge2;
    corner = _corner;
    return *this;
  }

  AreaLight& setAmbient(float a) {
    ambient = a;
    return *this;
  }

  AreaLight& setIntensity(float i) {
    intensity = i;
    return *this;
  }
};

}  // namespace RayTracingHistory
