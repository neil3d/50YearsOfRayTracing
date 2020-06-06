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
#include "MyAnimator.h"

namespace RayTracingHistory {

class MySceneObject {
 public:
  typedef std::shared_ptr<MySceneObject> Ptr;
  MySceneObject(const std::string& szName) : mName(szName) {}
  virtual ~MySceneObject() = default;

  virtual bool intersect(const Ray& ray, float tMin, float tMax,
                         HitRecord& outRec) = 0;

  template <typename T, typename... Args>
  T& createMaterial(Args&&... args) {
    auto newMtl = std::make_shared<T>(std::forward<Args>(args)...);
    mMaterial = newMtl;
    return *newMtl;
  }

  void setMaterial(MyMaterial::Ptr pMtl) { mMaterial = pMtl; }

  MySceneObject& setPosition(const glm::vec3& pos) {
    mTransform.setPosition(pos);
    return *this;
  }

  MySceneObject& setRotation(float pitch, float yaw, float roll) {
    mTransform.setRotation(pitch, yaw, roll);
    return *this;
  }

  MyAnimator& createAnimator(bool addInitKey) {
    mAnimator = std::make_shared<MyAnimator>();
    if (addInitKey) mAnimator->addKey(0, mTransform);
    return *mAnimator;
  }

  const std::string& getName() const { return mName; }

 protected:
  Ray _makeLocalRay(const Ray& worldRay) {
    glm::vec4 wo(worldRay.origin, 1.0f);
    glm::vec4 wd(worldRay.direction, 0.0f);
    const glm::mat4& world2Local = mTransform.getWorld2Local();
    return Ray(world2Local * wo, world2Local * wd);
  }
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
  MyAnimator::Ptr mAnimator;

 public:
  MySceneObject(const MySceneObject&) = delete;
  MySceneObject& operator=(const MySceneObject&) = delete;
};
}  // namespace RayTracingHistory
