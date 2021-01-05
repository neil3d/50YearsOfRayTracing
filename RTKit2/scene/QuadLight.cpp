#include "QuadLight.h"

#include "MyScene.h"

namespace RTKit2 {

static unsigned QUAD_INDICES[4] = {0, 1, 2, 3};

QuadLight::~QuadLight() {
  if (mGeometry) {
    rtcReleaseGeometry(mGeometry);
    mGeometry = nullptr;
  }
}

glm::vec3 QuadLight::sample(const glm::vec2& xi) const {
  return mCorner + xi.x * mEdge1 + xi.y * mEdge2;
}

void QuadLight::create(glm::vec3 a, glm::vec3 ab, glm::vec3 ac,
                       glm::vec3 intensity) {
  mCorner = a;
  mEdge1 = ab;
  mEdge2 = ac;
  mIntensity = intensity;

  // |a x b| = |a||b|sin(q)
  mArea = glm::length(glm::cross(ab, ac));
  mNormal = glm::normalize(glm::cross(ab, ac));

  // make geometry
  RTCScene rtcScene = mOwner->getRTCScene();
  RTCDevice rtcDevice = rtcGetSceneDevice(rtcScene);

  mGeometry = rtcNewGeometry(rtcDevice, RTC_GEOMETRY_TYPE_QUAD);
  rtcSetSharedGeometryBuffer(mGeometry, RTC_BUFFER_TYPE_INDEX, 0,
                             RTC_FORMAT_UINT4, QUAD_INDICES, 0,
                             4 * sizeof(unsigned int), 1);

  // quad vertices
  glm::vec3* vb = (glm::vec3*)rtcSetNewGeometryBuffer(
      mGeometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
      3 * sizeof(float), 4);

  for (int i = 0; i < 4; i++) vb[i] = getQuadVertex(i);

  // embree commit
  rtcCommitGeometry(mGeometry);
  mRTCID = rtcAttachGeometry(rtcScene, mGeometry);
}

glm::vec3 QuadLight::getQuadVertex(int i) const {
  glm::vec3 ret(0);
  switch (i) {
    case 0:
      ret = mCorner;
      break;
    case 1:
      ret = mCorner + mEdge1;
      break;
    case 2:
      ret = mCorner + mEdge1 + mEdge2;
      break;
    case 3:
      ret = mCorner + mEdge2;
      break;
  }
  return ret;
}

HitRecord QuadLight::makeHitRecord(const MyRay& ray, unsigned int primID,
                                   float t, glm::vec2 uv, glm::vec3 N) {
  HitRecord hit = MySceneObject::makeHitRecord(ray, primID, t, uv, N);
  hit.isLight = true;
  return hit;
}

}  // namespace RTKit2