#pragma once
#include <vector>

#include "MySceneObject.h"

namespace RTKit2 {

class MyGeometry : public MySceneObject {
 public:
  typedef std::shared_ptr<MyGeometry> Ptr;

  using MySceneObject::MySceneObject;

  virtual ~MyGeometry();

  void createFromBuffers(const std::vector<glm::vec3>& vertices,
                         const std::vector<unsigned>& indices);

  void createPlane(glm::vec3 a, glm::vec3 ab, glm::vec3 ac);

 protected:
  RTCGeometry mGeometry = nullptr;
};

}  // namespace RTKit2