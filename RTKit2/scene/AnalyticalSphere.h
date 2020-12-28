#pragma once
#include "MySceneObject.h"

namespace RTKit2 {

class AnalyticalSphere : public MySceneObject {
 public:
  typedef std::shared_ptr<AnalyticalSphere> Ptr;

  using MySceneObject::MySceneObject;

  virtual ~AnalyticalSphere();

  void create(const glm::vec3& p, float r);

  const glm::vec3& getCenter() const { return mCenter; }
  float getRadius() const { return mRadius; }

 protected:
  glm::vec3 mCenter = {0, 0, 0};
  float mRadius = 1;
  RTCGeometry mGeometry = nullptr;
};

}  // namespace RTKit2
