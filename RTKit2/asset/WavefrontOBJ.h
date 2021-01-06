
/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */
#pragma once

#include <vector>

#include "../utility/AABBox.h"
#include "MyAssetObject.h"

namespace RTKit2 {
class WavefrontOBJ : public MyAssetObject {
 public:
  typedef std::shared_ptr<WavefrontOBJ> Ptr;

  struct Face {
    unsigned int vertexIndex[3];
    unsigned int normalIndex[3];
    unsigned int texcoordIndex[3];
    unsigned int materialID;
    glm::vec3 normal;
  };
  struct MtlDesc {
    std::string name;
    glm::vec3 emission = {0, 0, 0};
    glm::vec3 diffuse = {1, 1, 1};
    glm::vec3 specular = {0, 0, 0};
    float shininess = 1;
    float ior = 1;  // index of refraction
  };
  struct SubMesh {
    std::vector<Face> faces;
    std::vector<unsigned int> indices;  // index buffer
  };

 public:
  using MyAssetObject::MyAssetObject;

  virtual void loadFromFile(const std::string& szPath) override;

  const std::vector<SubMesh>& getSubMeshes() const { return mSubMeshes; }

  const std::vector<MtlDesc>& getMaterials() const { return mMaterials; }

 private:
  void _generateFaceNormal();

  void _buildBoundingBox();

 private:
  AABBox mBoundingBox;

  // vertex buffer
  std::vector<glm::vec3> mVertices;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec2> mTexcoords;

  // sub mesh / shapes
  std::vector<SubMesh> mSubMeshes;

  // materials
  std::vector<MtlDesc> mMaterials;
};
}  // namespace RTKit2