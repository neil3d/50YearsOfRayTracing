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
class MaterialImporter;

class MeshInstance : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  MeshInstance& setMeshFile(const std::string& szObjFileName);
  MeshInstance& createDynamic(const std::vector<glm::vec3>& vertices, const std::vector<int>& indices);

  size_t importMaterial(MaterialImporter* pImporter);

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) override;

 private:
  std::shared_ptr<TriangleMesh> mMesh;
  std::vector<MyMaterial::Ptr> mModelMtls;
};
}  // namespace RayTracingHistory
