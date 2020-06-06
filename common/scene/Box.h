/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <array>

#include "MySceneObject.h"
#include "Parallelogram.h"

namespace RayTracingHistory {

class Box : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) override;

  Box& setExtents(float x, float y, float z) {
    mExtents = glm::vec3(x, y, z);
    return *this;
  }

  private:
  void _updateSides();

 protected:
  bool mDirty = true;
  glm::vec3 mExtents = glm::vec3(1);
  std::array<Parallelogram::Ptr, 6> mSides;
};
}  // namespace RayTracingHistory
