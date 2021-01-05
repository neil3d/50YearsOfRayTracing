#pragma once
#include "MySceneObject.h"

namespace RTKit2 {

class TriangulatedSphere : public MySceneObject {
 public:
  typedef std::shared_ptr<TriangulatedSphere> Ptr;

  using MySceneObject::MySceneObject;

  virtual ~TriangulatedSphere();

  void create(const glm::vec3& p, float r, int numPhi = 64, int numTheta = 128);

  const glm::vec3& getCenter() const { return mCenter; }
  float getRadius() const { return mRadius; }

 protected:
  glm::vec3 mCenter = {0, 0, 0};
  float mRadius = 1;
  RTCGeometry mGeometry = nullptr;
};

}  // namespace RTKit2
