#pragma once
#include <vector>

#include "../math/Transform.h"
#include "../math/AABBox.h"
#include "MySceneObject.h"

class Mesh : public MySceneObject {
 public:
  using MySceneObject::MySceneObject;

  Mesh& loadWavefrontObj(const std::string& szFileName);

  Mesh& setPosition(glm::vec3 pos);
  Mesh& setScale(glm::vec3 scale);
  Mesh& setRotation(float pitch, float yaw, float roll);

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) override;

 private:
  struct Face {
    int vertexIndex[3];
    int normalIndex[3];
    int texcoordIndex[3];
  };

  struct SubMesh {
    std::string name;
    std::vector<Face> faces;
    std::vector<int> materialIDs;  // per-face material ID
  };

 private:
  Transform mTransform;
  AABBox mBoundingBox;  // local space

  std::vector<glm::vec3> mVertices;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec2> mTexcoords;
  std::vector<SubMesh> mSubMeshes;
};