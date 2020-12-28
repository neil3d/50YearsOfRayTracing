#pragma once

#include "MySceneObject.h"

namespace RTKit2 {

class QuadLight : public MySceneObject {
 public:
  typedef std::shared_ptr<QuadLight> Ptr;

  using MySceneObject::MySceneObject;
  virtual ~QuadLight();

  void create(glm::vec3 a, glm::vec3 ab, glm::vec3 ac, glm::vec3 intensity);

  glm::vec3 getIntensity() const { return mIntensity; }
  glm::vec3 getQuadVertex(int i) const;
  float getArea() const { return mArea; }
  glm::vec3 getNormal() const { return mNormal; }

  glm::vec3 sample(const glm::vec2& xi) const;

 private:
  glm::vec3 mCorner, mEdge1, mEdge2;
  glm::vec3 mIntensity = {1, 1, 1};
  RTCGeometry mGeometry = nullptr;

  glm::vec3 mNormal;
  float mArea = 1.0f;
};

}  // namespace RTKit2