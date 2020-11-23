#include "MyScene.h"

namespace RTKit1 {

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

}  // namespace RTKit1
