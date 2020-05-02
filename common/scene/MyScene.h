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

  template <typename T>
  T& createObject(const std::string& szName) {
    auto newObject = std::make_shared<T>(szName);
    mObjects.emplace_back(newObject);
    return *newObject;
  }

  virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& outRec);

 protected:
  std::list<MySceneObject::Ptr> mObjects;

 public:
  MyScene(const MyScene&) = delete;
  MyScene& operator=(const MyScene&) = delete;
};