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
  mGeometryDict.insert(std::make_pair(ID, obj));
}

void MyScene::attachLight(QuadLight::Ptr lgt) {
  unsigned int ID = lgt->getRTCID();
  mLightDict.insert(std::make_pair(ID, lgt));
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

  auto hitGeom = mGeometryDict.find(rayhit.hit.geomID);
  if (hitGeom != mGeometryDict.end()) {
    outRec.isLight = false;
    outRec.t = rayhit.ray.tfar;
    outRec.pt = ray.getPoint(outRec.t);
    outRec.uv.x = rayhit.hit.u;
    outRec.uv.y = rayhit.hit.v;
    outRec.obj = hitGeom->second.get();
    outRec.mtl = outRec.obj->getMaterial();

    // unnormalized geometry normal in object space
    glm::vec3 localN(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);

    glm::vec4 HN = outRec.obj->getTransform().getNormalMatrix() *
                   glm::vec4(glm::normalize(localN), 0);
    outRec.normal = glm::normalize(glm::vec3(HN));

    return true;
  }  // end of if

  auto hitLgt = mLightDict.find(rayhit.hit.geomID);
  if (hitLgt != mLightDict.end()) {
    outRec.isLight = true;
    outRec.t = rayhit.ray.tfar;
    outRec.pt = ray.getPoint(outRec.t);
    outRec.obj = hitLgt->second.get();
    outRec.mtl = outRec.obj->getMaterial();
    return true;
  }  // end of if

  std::cerr << "Geometry ID not found." << std::endl;
  return false;
}

QuadLight::Ptr MyScene::getFirstLight() const {
  if (mLightDict.empty()) return QuadLight::Ptr();

  auto first = mLightDict.begin();
  return first->second;
}

std::vector<QuadLight::Ptr> MyScene::getLights() const {
  std::vector<QuadLight::Ptr> lights;
  for (auto pair : mLightDict) lights.push_back(pair.second);
  return lights;
}

}  // namespace RTKit2