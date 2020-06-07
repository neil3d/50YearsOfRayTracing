/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <tuple>
#include <vector>

#include "../geometry/AABBox.h"
#include "MyAssetObject.h"
#include "MaterialImporter.h"

namespace RayTracingHistory {
class TriangleMesh : public MyAssetObject {
 public:
  using MyAssetObject::MyAssetObject;

  // load Wavefront .Obj file
  virtual void loadFromFile(const std::string& szPath) override;

  std::tuple<bool, float, glm::vec3, glm::vec2, int> intersect(const Ray& ray,
                                                          float tMin,
                                                          float tMax);
  
  std::vector<MyMaterial::Ptr> importMaterial(MaterialImporter* pImporter);

 private:
  void _generateFaceNormal();

  void _buildBoundingBox();

 private:
  struct Face {
    int vertexIndex[3];
    int normalIndex[3];
    int texcoordIndex[3];
    int materialID;
    glm::vec3 normal;
  };

  struct MtlDesc {
    std::string name;
  };

 private:
  std::vector<glm::vec3> mVertices;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec2> mTexcoords;
  std::vector<Face> mFaces;

  std::vector<MtlDesc> mMaterials;

  AABBox mBoundingBox;
};
}  // namespace RayTracingHistory