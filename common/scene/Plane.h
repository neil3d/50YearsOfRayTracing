/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "MySceneObject.h"

namespace RayTracingHistory {

class Plane : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

  // (p-p0).n = 0
  Plane& setPointNormal(const glm::vec3& pt, const glm::vec3& n) {
    setPosition(pt);
    mNormal = n;
    return *this;
  }

 protected:
  glm::vec3 mNormal = {0, 1, 0};
};
}  // namespace RayTracingHistory
