#include "MyGeometry.h"

#include "MyScene.h"

namespace RTKit2 {

MyGeometry::~MyGeometry() {
  if (mGeometry) {
    rtcReleaseGeometry(mGeometry);
    mGeometry = nullptr;
  }
}

void MyGeometry::createFromBuffers(const std::vector<glm::vec3>& vertices,
                                   const std::vector<unsigned>& indices) {
  RTCScene rtcScene = mOwner->getRTCScene();
  RTCDevice rtcDevice = rtcGetSceneDevice(rtcScene);
  mGeometry = rtcNewGeometry(rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE);

  // create & fill vertex buffer
  glm::vec3* vb = (glm::vec3*)rtcSetNewGeometryBuffer(
      mGeometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
      3 * sizeof(float), vertices.size());
  for (size_t i = 0; i < vertices.size(); i++) vb[i] = vertices[i];

  // create & fill index buffer
  unsigned* ib = (unsigned*)rtcSetNewGeometryBuffer(
      mGeometry, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
      3 * sizeof(unsigned), indices.size() / 3);
  for (size_t i = 0; i < indices.size(); i++) ib[i] = indices[i];

  // embree commit
  rtcCommitGeometry(mGeometry);
  mRTCID = rtcAttachGeometry(rtcScene, mGeometry);
}

static unsigned int PLANE_INDICES[4] = {0, 1, 2, 3};

void MyGeometry::createPlane(glm::vec3 a, glm::vec3 ab, glm::vec3 ac) {
  // make geometry
  RTCScene rtcScene = mOwner->getRTCScene();
  RTCDevice rtcDevice = rtcGetSceneDevice(rtcScene);

  mGeometry = rtcNewGeometry(rtcDevice, RTC_GEOMETRY_TYPE_QUAD);
  rtcSetSharedGeometryBuffer(mGeometry, RTC_BUFFER_TYPE_INDEX, 0,
                             RTC_FORMAT_UINT4, PLANE_INDICES, 0,
                             4 * sizeof(unsigned int), 1);

  // quad vertices
  glm::vec3* vb = (glm::vec3*)rtcSetNewGeometryBuffer(
      mGeometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
      3 * sizeof(float), 4);
  vb[0] = a;
  vb[1] = a + ab;
  vb[2] = a + ab + ac;
  vb[3] = a + ac;

  // embree commit
  rtcCommitGeometry(mGeometry);
  mRTCID = rtcAttachGeometry(rtcScene, mGeometry);
}

}  // namespace RTKit2