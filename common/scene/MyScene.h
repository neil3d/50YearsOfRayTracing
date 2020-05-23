#pragma once
#include <list>
#include <memory>
#include <string>

#include "MySceneObject.h"

class MyScene {
 public:
  typedef std::shared_ptr<MyScene> Ptr;

  MyScene() = default;
  virtual ~MyScene() = default;

  virtual void init() = 0;

  template <typename T, typename... Args>
  T& createObject(Args&&... args) {
    auto newObject = std::make_shared<T>(std::forward<Args>(args)...);
    mObjects.emplace_back(newObject);
    return *newObject;
  }

  virtual bool closestHit(const Ray& ray, float tMin, float tMax, HitRecord& outRec);

 protected:
  std::list<MySceneObject::Ptr> mObjects;

 public:
  MyScene(const MyScene&) = delete;
  MyScene& operator=(const MyScene&) = delete;
};