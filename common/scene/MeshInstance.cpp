#include "MeshInstance.h"

#include "../asset/MyAssetManager.h"
#include "../asset/TriangleMesh.h"


namespace RayTracingHistory {

MeshInstance& MeshInstance::setMeshFile(const std::string& szObjFileName) {
  mMesh = MyAssetManager::instance().loadAssetObject<TriangleMesh>(szObjFileName);
  return *this;
}

bool MeshInstance::intersect(const Ray& ray, float tMin, float tMax,
                       HitRecord& outRec) {
  // tranform ray to local space
  Ray localRay = ray;

  // bouding box check
  // if (!mBoundingBox.hit(localRay, tMin, tMax)) return false;

  // trangle list check

  return false;
}
}  // namespace RayTracingHistory
