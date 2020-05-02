#pragma once
#include "MySceneObject.h"

class Sphere : public MySceneObject {
 public:
  Sphere(const std::string& szName) : MySceneObject(szName) {}

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