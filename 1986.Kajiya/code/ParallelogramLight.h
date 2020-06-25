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
  glm::vec3 mNormal;

 public:
  using AreaLight::AreaLight;

  virtual std::tuple<Ray, float, glm::vec3> generateShadowRay(
      const glm::vec3& shadingPt, const glm::vec3& normal,
      const glm::vec2& xi) const override {
    glm::vec3 origin = shadingPt;
    glm::vec3 pos = mCorner + xi.x * mEdge1 + xi.y * mEdge2;
    glm::vec3 L = pos - origin;
    return std::make_tuple(Ray(origin, L), glm::length(L), mNormal);
    ;
  }

  ParallelogramLight& setShape(const glm::vec3& edge1, const glm::vec3& edge2,
                               const glm::vec3& corner) {
    mEdge1 = edge1;
    mEdge2 = edge2;
    mCorner = corner;

    mNormal = glm::normalize(glm::cross(edge1, edge2));

    // |a x b| = |a||b|sin(q)
    mArea = glm::length(glm::cross(edge1, edge2));
    return *this;
  }

  float getArea() const { return mArea; }
};
}  // namespace RayTracingHistory
