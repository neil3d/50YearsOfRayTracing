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

#include "MySceneObject.h"

namespace RayTracingHistory {

class TriangleMesh;

class MeshInstance : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  MeshInstance& setMeshFile(const std::string& szObjFileName);

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) override;

 private:
  std::shared_ptr<TriangleMesh> mMesh;
};
}  // namespace RayTracingHistory
