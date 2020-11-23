/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
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

namespace RTKit1 {

class MySceneObject {
 public:
  struct UserData {};

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

  MySceneObject& setRotation(const glm::quat& rot) {
    mTransform.setRotation(rot);
    return *this;
  }
  MySceneObject& setScale(float s) {
    mTransform.setScale(glm::vec3(s, s, s));
    return *this;
  }

  MySceneObject& setScale(glm::vec3 s) {
    mTransform.setScale(s);
    return *this;
  }

  MyAnimator& createAnimator(bool addInitKey) {
    mAnimator = std::make_shared<MyAnimator>();
    if (addInitKey) mAnimator->addKey(0, mTransform);
    return *mAnimator;
  }

  const std::string& getName() const { return mName; }

  void setUserData(std::shared_ptr<UserData> userData) { mUserData = userData; }

  std::shared_ptr<UserData> getUserData() const { return mUserData; }

 protected:
  Ray _makeLocalRay(const Ray& worldRay, Transform* animTrans) {
    glm::vec4 wo(worldRay.origin, 1.0f);
    glm::vec4 wd(worldRay.direction, 0.0f);
    if (animTrans) {
      const glm::mat4& world2Local = animTrans->getWorld2Local();
      return Ray(world2Local * wo, world2Local * wd, false);
    } else {
      const glm::mat4& world2Local = mTransform.getWorld2Local();
      return Ray(world2Local * wo, world2Local * wd, false);
    }
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
  std::shared_ptr<UserData> mUserData;

 public:
  MySceneObject(const MySceneObject&) = delete;
  MySceneObject& operator=(const MySceneObject&) = delete;
};
}  // namespace RTKit1
