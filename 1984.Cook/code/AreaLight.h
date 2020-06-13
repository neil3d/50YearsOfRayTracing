/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <algorithm>
#include <glm/glm.hpp>

#include "geometry/Ray.h"

namespace RayTracingHistory {

struct AreaLight {
  glm::vec3 edge1, edge2;
  glm::vec3 corner;

  float ambient = 0.1f;
  float intensity = 2.0f;
  float range = 10;

  Ray jitteredShadowRay(const glm::vec3& shadingPt, const glm::vec2& xi) const {
    glm::vec3 pos = corner + xi.x * edge1 + xi.y * edge2;
    glm::vec3 L = pos - shadingPt;
    return Ray(shadingPt, L);
  }

  float lighting(const glm::vec3& shadingPt, const glm::vec3& normal,
                 const glm::vec3& viewDir, const glm::vec2& xi) const {
    glm::vec3 pos = corner + xi.x * edge1 + xi.y * edge2;

    glm::vec3 L = glm::normalize(pos - shadingPt);
    glm::vec3 H = glm::normalize(L - viewDir);
    float NdotH = glm::dot(normal, H);
    float NdotL = glm::dot(normal, L);

    float falloff = 1.0f;
    float d = glm::distance(pos, shadingPt);
    if (d > range) {
      float r = range / d;
      falloff = r * r;
    }

    float diffuse = std::max(0.0f, NdotL) * falloff;
    return intensity * (ambient + diffuse);
  }

  AreaLight& setParallelogram(const glm::vec3& _edge1, const glm::vec3& _edge2,
                              const glm::vec3& _corner) {
    edge1 = _edge1;
    edge2 = _edge2;
    corner = _corner;
    return *this;
  }

  AreaLight& setRange(float val) {
    range = val;
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
