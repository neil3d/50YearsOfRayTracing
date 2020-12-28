#pragma once
#include <embree3/rtcore.h>

#include <map>
#include <memory>
#include <vector>

#include "../framework/MyRay.h"
#include "HitRecord.h"
#include "MySceneObject.h"
#include "QuadLight.h"

namespace RTKit2 {

class MyScene {
 public:
  typedef std::shared_ptr<MyScene> Ptr;

  MyScene() = default;
  virtual ~MyScene();

  MyScene(RTCScene rtcScene) : mScene(rtcScene) {}

  bool intersect(const MyRay& ray, float tNear, float tFar,
                 HitRecord& outRec) const;
                 
  bool occluded(const MyRay& ray, float tNear, float tFar) const;

  RTCScene getRTCScene() { return mScene; }

  void attachLight(QuadLight::Ptr lgt);

  void attachGeometry(MySceneObject::Ptr obj);

  void rtcCommit();

  QuadLight::Ptr getFirstLight() const;

  std::vector<QuadLight::Ptr> getLights() const;

 protected:
  RTCScene mScene = nullptr;
  std::map<unsigned int, MySceneObject::Ptr> mGeometryDict;
  std::map<unsigned int, QuadLight::Ptr> mLightDict;

 public:
  MyScene(const MyScene&) = delete;
  MyScene& operator=(const MyScene&) = delete;
};
}  // namespace RTKit2