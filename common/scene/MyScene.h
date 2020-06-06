/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>

#include "MySceneObject.h"

namespace RayTracingHistory {

class MyScene {
 public:
  typedef std::shared_ptr<MyScene> Ptr;

  MyScene() = default;
  virtual ~MyScene() = default;

  virtual void init() = 0;
  virtual void onObjectCreated(MySceneObject* newObject) {}

  template <typename T, typename... Args>
  T& createObject(Args&&... args) {
    auto newObject = std::make_shared<T>(std::forward<Args>(args)...);
    mObjects.emplace_back(newObject);
    
    onObjectCreated(newObject.get());
    return *newObject;
  }

  bool closestHit(const Ray& ray, float tMin, float tMax,
                  HitRecord& outRec) const;

  template <typename T>
  bool anyHit(const Ray& ray, float tMin, float tMax, T callback) const {
    bool hitAnything = false;
    HitRecord rec;

    for (auto& obj : mObjects) {
      if (obj->intersect(ray, tMin, tMax, rec)) {
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

}  // namespace RayTracingHistory
