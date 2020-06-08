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
#include <tuple>
#include <vector>

#include "../geometry/AABBox.h"
#include "MaterialImporter.h"
#include "MyAssetObject.h"

namespace RayTracingHistory {
class TriangleMesh : public MyAssetObject {
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

  struct BVHNode {
    std::vector<int> faceList;
    AABBox boudingBox;

    std::unique_ptr<BVHNode> leftChild;
    std::unique_ptr<BVHNode> rightChild;
  };


 public:
  using MyAssetObject::MyAssetObject;

  // load Wavefront .Obj file
  virtual void loadFromFile(const std::string& szPath) override;

  std::tuple<bool, float, glm::vec3, glm::vec2, int> intersect(const Ray& ray,
                                                               float tMin,
                                                               float tMax);

  std::vector<MyMaterial::Ptr> importMaterial(MaterialImporter* pImporter);

 private:
  std::tuple<bool, float, glm::vec3, glm::vec2, int> _perFaceIntersect(
      const Ray& ray, float tMin, float tMax);

  std::tuple<bool, float, glm::vec3, glm::vec2, int> _accelIntersect(
      const BVHNode* pNode,
      const Ray& ray, float tMin, float tMax);

  std::tuple<bool, float, glm::vec3, glm::vec2, int> _faceIntersect(
      const Face& face, const Ray& ray, float tMin, float tMax);

  void _generateFaceNormal();

  void _buildBoundingBox();

  void _buildBVH(BVHNode* pNode, std::vector<int> faceList);

  AABBox _buildBoundingBox(const std::vector<int>& faceList);

 private:
  std::vector<glm::vec3> mVertices;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec2> mTexcoords;
  std::vector<Face> mFaces;

  std::vector<MtlDesc> mMaterials;

  AABBox mBoundingBox;

  BVHNode mBVHRoot;
};
}  // namespace RayTracingHistory