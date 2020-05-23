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

  bool closestHit(const Ray& ray, float tMin, float tMax, HitRecord& outRec);

  template <typename T>
  bool anyHit(const Ray& ray, float tMin, float tMax, T callback) {
    bool hitAnything = false;
    HitRecord rec;

    for (auto& obj : mObjects) {
      if (obj->hit(ray, tMin, tMax, rec)) {
        hitAnything = true;
        if (callback(rec)) break;
      }
    }  // end of for

    return hitAnything;
  }

 protected:
  std::list<MySceneObject::Ptr> mObjects;

 public:
  MyScene(const MyScene&) = delete;
  MyScene& operator=(const MyScene&) = delete;
};