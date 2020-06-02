/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "Plane.h"

namespace RayTracingHistory {

class Parallelogram : public Plane {
 public:
  using Plane::Plane;

  Parallelogram& setEdges(glm::vec3 e1, glm::vec3 e2) {
    edge1 = e1;
    edge2 = e2;

    len1 = glm::length(e1);
    len2 = glm::length(e2);

    // update plane normal
    mNormal = glm::normalize(glm::cross(e1, e2));
    return *this;
  }

  Parallelogram& setAnchor(glm::vec3 anchor) {
    setPosition(anchor);
    return *this;
  }

  glm::vec3 getAnchor() const { return mTransform.getPosition(); }

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  glm::vec3 edge1 = {1, 0, 0};
  float len1 = 1;

  glm::vec3 edge2 = {0, 0, 1};
  float len2 = 1;
};
}  // namespace RayTracingHistory
