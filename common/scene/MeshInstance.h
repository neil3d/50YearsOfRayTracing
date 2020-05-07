#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../asset/Mesh.h"
#include "../geometry/Transform.h"
#include "MySceneObject.h"

class MeshInstance : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  MeshInstance& setMeshFile(const std::string& szObjFileName);
  MeshInstance& setPosition(glm::vec3 pos);
  MeshInstance& setScale(glm::vec3 scale);
  MeshInstance& setRotation(float pitch, float yaw, float roll);

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  Transform mTransform;
  std::shared_ptr<Mesh> mMesh;
};