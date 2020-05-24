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

class Sphere : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  Sphere& setCenter(const glm::vec3& center) {
    mCenter = center;
    return *this;
  }

  Sphere& setRadius(float radius) {
    mRadius = radius;
    return *this;
  }

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  glm::vec3 mCenter;
  float mRadius = 1;
};

}  // namespace RayTracingHistory
