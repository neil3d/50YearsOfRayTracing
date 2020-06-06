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

class Box : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) override;

 protected:
};
}  // namespace RayTracingHistory
