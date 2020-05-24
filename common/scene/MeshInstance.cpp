#include "MeshInstance.h"

#include "../asset/MyAssetManager.h"

namespace RayTracingHistory {

MeshInstance& MeshInstance::setMeshFile(const std::string& szObjFileName) {
  mMesh = MyAssetManager::instance().loadAssetObject<Mesh>(szObjFileName);
  return *this;
}

MeshInstance& MeshInstance::setPosition(glm::vec3 pos) {
  mTransform.setPosition(pos);
  return *this;
}

MeshInstance& MeshInstance::setScale(glm::vec3 scale) {
  mTransform.setScale(scale);
  return *this;
}

MeshInstance& MeshInstance::setRotation(float pitch, float yaw, float roll) {
  mTransform.setRotation(pitch, yaw, roll);
  return *this;
}

bool MeshInstance::hit(const Ray& ray, float tMin, float tMax,
                       HitRecord& outRec) {
  // tranform ray to local space
  Ray localRay = ray;

  // bouding box check
  // if (!mBoundingBox.hit(localRay, tMin, tMax)) return false;

  // trangle list check

  return false;
}
}  // namespace RayTracingHistory
