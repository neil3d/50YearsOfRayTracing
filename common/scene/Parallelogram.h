#pragma once
#include "MySceneObject.h"

class Parallelogram : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  Parallelogram& setParam(glm::vec3 _v1, glm::vec3 _v2, glm::vec3 _anchor) {
    v1 = _v1;
    v2 = _v2;
    anchor = _anchor;

    glm::vec3 normal = glm::normalize(glm::cross(v2, v1));
    float d = glm::dot(normal, anchor);
    this->v1 *= 1.0f / glm::dot(v1, v1);
    this->v2 *= 1.0f / glm::dot(v2, v2);
    plane = glm::vec4(normal, d);

    return *this;
  }

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  glm::vec4 plane;
  glm::vec3 v1;
  glm::vec3 v2;
  glm::vec3 anchor;
};