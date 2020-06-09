#include "MeshInstance.h"

#include "../asset/MyAssetManager.h"
#include "../asset/TriangleMesh.h"

namespace RayTracingHistory {

MeshInstance& MeshInstance::setMeshFile(const std::string& szObjFileName) {
  mMesh =
      MyAssetManager::instance().loadAssetObject<TriangleMesh>(szObjFileName);
  return *this;
}

size_t MeshInstance::importMaterial(MaterialImporter* pImporter) {
  if (!mMesh) return 0;

  mModelMtls = mMesh->importMaterial(pImporter);
  if (!mModelMtls.empty()) mMaterial = mModelMtls[0];

  return mModelMtls.size();
}

bool MeshInstance::intersect(const Ray& ray, float tMin, float tMax,
                             HitRecord& outRec) {
  if (!mMesh) return false;

  // tranform ray to local space
  Ray localRay = _makeLocalRay(ray, nullptr);

  // trangle mesh check
  auto result = mMesh->intersect(localRay, tMin, tMax);

  bool hitAnyFace = std::get<0>(result);
  if (hitAnyFace) {
    float tnear = std::get<1>(result);
    glm::vec3 hitNormal = std::get<2>(result);
    glm::vec2 hitUV = std::get<3>(result);
    int mtlID = std::get<4>(result);

    glm::vec3 WN(mTransform.getNormalMatrix() * glm::vec4(hitNormal, 0));

    outRec = _makeHitRecord(ray, tnear, WN, hitUV);

    if (mtlID != -1) outRec.mtl = mModelMtls[mtlID].get();
  }

  return hitAnyFace;
}
}  // namespace RayTracingHistory
