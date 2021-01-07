#pragma once
#include <vector>

#include "../asset/WavefrontOBJ.h"
#include "MySceneObject.h"

namespace RTKit2 {

class TriangleMesh : public MySceneObject {
 public:
  typedef std::shared_ptr<TriangleMesh> Ptr;

  using MySceneObject::MySceneObject;

  virtual ~TriangleMesh();

  void createFromObj(WavefrontOBJ::Ptr model, int subMesh,
                     const Transform& trans);

  void setMaterialBundle(const std::vector<MyMaterial::Ptr> mtlBundle) {
    mMaterialBundle = mtlBundle;
  }

  virtual HitRecord makeHitRecord(const MyRay& ray, unsigned int primID, float t, glm::vec2 uv, glm::vec3 N) override;

 protected:
  WavefrontOBJ::Ptr mModel;
  std::vector<MyMaterial::Ptr> mMaterialBundle;
  int mSubMeshIndex = 0;
  glm::mat4 mNormalMatrix;

  RTCGeometry mGeometry = nullptr;
};

}  // namespace RTKit2