/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "MySceneObject.h"

namespace RTKit1 {

class Sphere : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  Sphere& setCenter(const glm::vec3& center) {
    setPosition(center);
    return *this;
  }

  Sphere& setRadius(float radius) {
    mRadius = radius;
    return *this;
  }

  glm::vec3 getCenter() const{
    return mTransform.getPosition();
  }

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  float mRadius = 1;
};

}  // namespace RTKit1
