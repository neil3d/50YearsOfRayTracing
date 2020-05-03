#pragma once
#include "MySceneObject.h"

class Plane : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  // (p-p0).n = 0
  glm::vec3 mP0 = {0, 0, 0};
  glm::vec3 mNormal = {0, 1, 0};
};