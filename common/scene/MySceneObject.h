#pragma once
#include <memory>
#include <string>

#include "../material/MyMaterial.h"
#include "../math/Ray.h"
#include "HitRecord.h"

class MySceneObject {
 public:
  typedef std::shared_ptr<MySceneObject> Ptr;
  MySceneObject(const std::string& szName) : mName(szName) {}
  virtual ~MySceneObject() = default;

  virtual bool hit(const Ray& ray, float tMin, float tMax,
                   HitRecord& outRec) = 0;

 protected:
  std::string mName;
  MyMaterial::Ptr mMateril;

 public:
  MySceneObject(const MySceneObject&) = delete;
  MySceneObject& operator=(const MySceneObject&) = delete;
};