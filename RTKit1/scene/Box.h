/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <array>

#include "MySceneObject.h"
#include "Parallelogram.h"

namespace RTKit1 {

class Box : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) override;

  Box& setExtents(float x, float y, float z) {
    mExtents = glm::vec3(x, y, z);
    return *this;
  }

  void init();

 protected:
  glm::vec3 mExtents = glm::vec3(1);
  std::array<Parallelogram::Ptr, 6> mSides;
};
}  // namespace RTKit1
