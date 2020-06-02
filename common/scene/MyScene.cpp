#include "MyScene.h"

namespace RayTracingHistory {

void MyScene::setupMaterials(
    const std::map<std::string, MyMaterial::Ptr>& materialMap) {
  for (auto& obj : mObjects) {
    const std::string& szObjectName = obj->getName();

    auto iter = materialMap.find(szObjectName);
    if (iter != materialMap.end()) obj->setMaterial(iter->second);

  }  // end of foreach(scene object)
}

bool MyScene::closestHit(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) const {
  bool hitAnything = false;
  float closestSoFar = tMax;
  HitRecord rec;

  for (auto& obj : mObjects) {
    if (obj->intersect(ray, tMin, closestSoFar, rec)) {
      hitAnything = true;
      closestSoFar = rec.t;
      outRec = rec;
    }
  }  // end of for

  return hitAnything;
}

}  // namespace RayTracingHistory
