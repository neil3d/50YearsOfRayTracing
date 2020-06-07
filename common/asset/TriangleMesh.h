/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "../geometry/AABBox.h"
#include "MyAssetObject.h"

namespace RayTracingHistory {
class TriangleMesh : public MyAssetObject {
 public:
  using MyAssetObject::MyAssetObject;

  // load Wavefront .Obj file
  virtual void loadFromFile(const std::string& szPath) override;

 private:
  struct Face {
    int vertexIndex[3];
    int normalIndex[3];
    int texcoordIndex[3];
    int materialID;
  };

 private:
  std::vector<glm::vec3> mVertices;
  std::vector<glm::vec3> mNormals;
  std::vector<glm::vec2> mTexcoords;
  std::vector<Face> mFaces;

  AABBox mBoundingBox;
};
}  // namespace RayTracingHistory