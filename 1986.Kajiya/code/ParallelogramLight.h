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
  glm::vec3 mEdge1, mEdge2;
  glm::vec3 mCorner;

  float mArea = 1.0f;

 public:
  using AreaLight::AreaLight;

  virtual Ray generateShadowRay(const glm::vec3& shadingPt,
                                const glm::vec2& xi) const override {
    glm::vec3 pos = mCorner + xi.x * mEdge1 + xi.y * mEdge2;
    glm::vec3 L = pos - shadingPt;
    return Ray(shadingPt, L);
  }

  ParallelogramLight& setShape(const glm::vec3& edge1, const glm::vec3& edge2,
                               const glm::vec3& corner) {
    mEdge1 = edge1;
    mEdge2 = edge2;
    mCorner = corner;

    // area = a*b*sin(x)
    float a = glm::length(edge1);
    float b = glm::length(edge2);
    float cosx = glm::dot(mEdge1, mEdge2) / (a * b);
    float sinx = sqrtf(1.0f - cosx * cosx);
    mArea = a * b * sinx;
    return *this;
  }

  float getArea() const { return mArea; }
};
}  // namespace RayTracingHistory
