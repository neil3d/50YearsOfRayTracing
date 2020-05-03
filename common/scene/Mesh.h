#pragma once
#include "MySceneObject.h"

class Mesh : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  Mesh& loadWavefrontObj(const std::string& szFileName);

  Mesh& setLocation(glm::vec3 loc);
  Mesh& setScale(glm::vec3 scale);
  Mesh& setRotation(float pitch, float yaw, float roll);

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  
};