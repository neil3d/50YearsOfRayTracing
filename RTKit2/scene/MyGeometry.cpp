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

void MyGeometry::createFromObj(const std::string& assetKey,
                               const glm::mat4& transform) {
                                 
                               }

}  // namespace RTKit2