#include "MeshInstance.h"

#include "../asset/MyAssetManager.h"
#include "../asset/TriangleMesh.h"

namespace RayTracingHistory {

MeshInstance& MeshInstance::setMeshFile(const std::string& szObjFileName) {
  mMesh =
      MyAssetManager::instance().loadAssetObject<TriangleMesh>(szObjFileName);
  return *this;
}

bool MeshInstance::intersect(const Ray& ray, float tMin, float tMax,
                             HitRecord& outRec) {
  if (!mMesh) return false;

  // tranform ray to local space
  Ray localRay = _makeLocalRay(ray, nullptr);

  // trangle mesh check
  auto result = mMesh->intersect(ray, tMin, tMax);

  bool hitAnyFace = std::get<0>(result);
  if (hitAnyFace) {
    float tnear = std::get<1>(result);
    glm::vec3 hitNormal = std::get<2>(result);
    glm::vec2 hitUV = std::get<3>(result);

    glm::vec3 WN(mTransform.getWorld2LocalT() * glm::vec4(hitNormal, 0));

    outRec = _makeHitRecord(ray, tnear, WN, hitUV);
  }

  return hitAnyFace;
}
}  // namespace RayTracingHistory
