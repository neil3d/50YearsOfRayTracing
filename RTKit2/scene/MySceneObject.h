/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <embree3/rtcore.h>

#include <memory>
#include <string>

#include "../framework/MyRay.h"
#include "../framework/MyTransform.h"
#include "../material/MyMaterial.h"
#include "HitRecord.h"

namespace RTKit2 {

constexpr unsigned int GEOMETRY_MASK = 0x01;
constexpr unsigned int LIGHT_MASK = 0x02;

class MyScene;

class MySceneObject {
 public:
  struct UserData {};

  typedef std::shared_ptr<MySceneObject> Ptr;
  MySceneObject(const std::string& szName, MyScene* owner)
      : mName(szName), mOwner(owner) {}
  virtual ~MySceneObject() = default;

  template <typename T, typename... Args>
  T& createMaterial(Args&&... args) {
    auto newMtl = std::make_shared<T>(std::forward<Args>(args)...);
    mMaterial = newMtl;
    return *newMtl;
  }

  void setMaterial(MyMaterial::Ptr pMtl) { mMaterial = pMtl; }

  MyMaterial* getMaterial() const { return mMaterial.get(); }

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

  void setTransformation(const Transform& trans) { mTransform = trans; }

  const Transform& getTransform() const { return mTransform; }

  const std::string& getName() const { return mName; }

  unsigned int getRTCID() const { return mRTCID; }

  void setUserData(std::shared_ptr<UserData> userData) { mUserData = userData; }

  std::shared_ptr<UserData> getUserData() const { return mUserData; }

 protected:
  MyScene* mOwner = nullptr;
  std::string mName;
  unsigned int mRTCID = RTC_INVALID_GEOMETRY_ID;
  MyMaterial::Ptr mMaterial;
  Transform mTransform;
  std::shared_ptr<UserData> mUserData;

 public:
  MySceneObject(const MySceneObject&) = delete;
  MySceneObject& operator=(const MySceneObject&) = delete;
};
}  // namespace RTKit2
