#include "MyScene.h"

bool MyScene::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRec) {
  bool hitAnything = false;
  float closestSoFar = tMax;
  HitRecord rec;

  for (auto& obj : mObjects) {
    if (obj->hit(ray, tMin, closestSoFar, rec)) {
      hitAnything = true;
      closestSoFar = rec.t;
      outRec = rec;
    }
  }  // end of for

  return hitAnything;
}
