/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>
#include <string>

#include "../geometry/Ray.h"
#include "../geometry/Transform.h"
#include "../material/MyMaterial.h"
#include "HitRecord.h"

namespace RayTracingHistory {

class MySceneObject {
 public:
  typedef std::shared_ptr<MySceneObject> Ptr;
  MySceneObject(const std::string& szName) : mName(szName) {}
  virtual ~MySceneObject() = default;

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) = 0;

  template <typename T, typename... Args>
  T& createMaterial(Args&&... args) {
    auto newMtl = std::make_shared<T>(std::forward<Args>(args)...);
    mMaterial = newMtl;
    return *newMtl;
  }

  MySceneObject& setPosition(const glm::vec3& pos) {
    mTransform.setPosition(pos);
    return *this;
  }

  MySceneObject& setRotation(float pitch, float yaw, float roll) {
    mTransform.setRotation(pitch, yaw, roll);
    return *this;
  }

 protected:
  HitRecord _makeHitRecord(const Ray& ray, const float t,
                           const glm::vec3& normal, const glm::vec2& uv) {
    HitRecord hit;
    hit.t = t;
    hit.p = ray.getPoint(t);
    hit.normal = normal;
    hit.uv = uv;

    hit.mtl = mMaterial.get();
    hit.obj = this;
    return hit;
  }

 protected:
  std::string mName;
  Transform mTransform;
  MyMaterial::Ptr mMaterial;

 public:
  MySceneObject(const MySceneObject&) = delete;
  MySceneObject& operator=(const MySceneObject&) = delete;
};
}  // namespace RayTracingHistory
