/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../asset/Mesh.h"
#include "../geometry/Transform.h"
#include "MySceneObject.h"

namespace RayTracingHistory {

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
  std::shared_ptr<Mesh> mMesh;
};
}  // namespace RayTracingHistory
