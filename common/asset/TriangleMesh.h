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
  struct Intersection {
    bool hit = false;
    float t = 0.0f;
    glm::vec3 normal = {0, 1, 0};
    glm::vec2 uv = {0, 0};
    int mtlID = -1;
  };

  using MyAssetObject::MyAssetObject;

  // load Wavefront .Obj file
  virtual void loadFromFile(const std::string& szPath) override;

  void createDynamic(const std::vector<glm::vec3>& vertices,
                     const std::vector<int>& indices);

  Intersection intersect(const Ray& ray, float tMin, float tMax);

  std::vector<MyMaterial::Ptr> importMaterial(MaterialImporter* pImporter);

 private:
  void _postMeshCreated();

  Intersection _perFaceIntersect(const Ray& ray, float tMin, float tMax);

  Intersection _accelIntersect(const BVHNode* pNode, const Ray& ray, float tMin,
                               float tMax);

  Intersection _faceIntersect(const Face& face, const Ray& ray, float tMin,
                              float tMax);

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