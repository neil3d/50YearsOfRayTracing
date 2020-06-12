/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "AreaLight.h"

namespace RayTracingHistory {

class ParallelogramLight : public AreaLight {
  glm::vec3 edge1, edge2;
  glm::vec3 corner;

  float mArea = 1.0f;

 public:
  using AreaLight::AreaLight;

  ParallelogramLight& setShape(const glm::vec3& _edge1, const glm::vec3& _edge2,
                               const glm::vec3& _corner) {
    edge1 = _edge1;
    edge2 = _edge2;
    corner = _corner;

    // area = a*b*sin(x)
    float a = glm::length(_edge1);
    float b = glm::length(_edge2);
    float cosx = glm::dot(edge1, edge2) / (a * b);
    float sinx = sqrtf(1.0f - cosx*cosx);
    mArea = a * b * sinx;
    return *this;
  }

  float getArea() const { return mArea; }
};
}  // namespace RayTracingHistory
