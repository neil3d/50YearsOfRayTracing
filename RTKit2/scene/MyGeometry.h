#pragma once
#include "MySceneObject.h"

namespace RTKit2 {

class MyGeometry : public MySceneObject {
 public:
  typedef std::shared_ptr<MyGeometry> Ptr;

  using MySceneObject::MySceneObject;

  virtual ~MyGeometry();

  void createFromBuffers(const std::vector<glm::vec3>& vertices,
                         const std::vector<unsigned>& indices);

 protected:
  RTCGeometry mGeometry = nullptr;
};

}  // namespace RTKit2