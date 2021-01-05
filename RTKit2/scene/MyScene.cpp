#include "MyScene.h"

#include <iostream>

namespace RTKit2 {
MyScene::~MyScene() {
  if (mScene) {
    rtcReleaseScene(mScene);
    mScene = nullptr;
  }
}

void MyScene::attachGeometry(MySceneObject::Ptr obj) {
  unsigned int ID = obj->getRTCID();
  mObjectDict.insert(std::make_pair(ID, obj));
}

void MyScene::attachLight(QuadLight::Ptr lgt) {
  unsigned int ID = lgt->getRTCID();
  mObjectDict.insert(std::make_pair(ID, lgt));
  mLights.push_back(lgt);
}

void MyScene::rtcCommit() { rtcCommitScene(mScene); }

bool MyScene::occluded(const MyRay& ray, float tNear, float tFar) const {
  RTCIntersectContext context;
  rtcInitIntersectContext(&context);

  RTCRay rtcRay;
  rtcRay.org_x = ray.origin.x;
  rtcRay.org_y = ray.origin.y;
  rtcRay.org_z = ray.origin.z;
  rtcRay.dir_x = ray.direction.x;
  rtcRay.dir_y = ray.direction.y;
  rtcRay.dir_z = ray.direction.z;
  rtcRay.tnear = tNear;
  rtcRay.tfar = tFar;
  rtcRay.mask = -1;
  rtcRay.flags = 0;
  rtcRay.time = 0;
  rtcRay.id = RTC_INVALID_GEOMETRY_ID;

  rtcOccluded1(mScene, &context, &rtcRay);
  return rtcRay.tfar != tFar;
}

bool MyScene::intersect(const MyRay& ray, float tNear, float tFar,
                        HitRecord& outRec) const {
  RTCIntersectContext context;
  rtcInitIntersectContext(&context);

  RTCRayHit rayhit;
  rayhit.ray.org_x = ray.origin.x;
  rayhit.ray.org_y = ray.origin.y;
  rayhit.ray.org_z = ray.origin.z;
  rayhit.ray.dir_x = ray.direction.x;
  rayhit.ray.dir_y = ray.direction.y;
  rayhit.ray.dir_z = ray.direction.z;
  rayhit.ray.tnear = tNear;
  rayhit.ray.tfar = tFar;
  rayhit.ray.mask = -1;
  rayhit.ray.flags = 0;
  rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

  rtcIntersect1(mScene, &context, &rayhit);

  if (rayhit.hit.geomID == RTC_INVALID_GEOMETRY_ID) return false;

  auto hitObj = mObjectDict.find(rayhit.hit.geomID);
  if (hitObj == mObjectDict.end()) {
    std::cerr << "Hit Object ID not found." << std::endl;
    return false;
  }

  outRec = hitObj->second->makeHitRecord(
      ray, rayhit.hit.primID, rayhit.ray.tfar,
      glm::vec2(rayhit.hit.u, rayhit.hit.v),
      glm::vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z));

  return true;
}

}  // namespace RTKit2