#include "MeshInstance.h"

#include "../asset/MyAssetManager.h"
#include "../asset/TriangleMesh.h"

namespace RTKit1 {

MeshInstance& MeshInstance::setMeshFile(const std::string& szObjFileName) {
  mMesh =
      MyAssetManager::instance().loadAssetObject<TriangleMesh>(szObjFileName);
  return *this;
}

MeshInstance& MeshInstance::createDynamic(
    const std::vector<glm::vec3>& vertices, const std::vector<int>& indices) {
  mMesh = std::make_shared<TriangleMesh>(mName + ".dynamic");
  mMesh->createDynamic(vertices, indices);

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

  if (result.hit) {
    glm::vec4 HN = mTransform.getNormalMatrix() * glm::vec4(result.normal, 0);
    glm::vec3 WN = glm::normalize(glm::vec3(HN));

    outRec = _makeHitRecord(ray, result.t, WN, result.uv);

    if (result.mtlID != -1 && !mModelMtls.empty())
      outRec.mtl = mModelMtls[result.mtlID].get();
  }

  return result.hit;
}
}  // namespace RTKit1
