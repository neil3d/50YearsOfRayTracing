#include "TriangulatedSphere.h"

#include "MyScene.h"

namespace RTKit2 {

TriangulatedSphere::~TriangulatedSphere() {
  if (mGeometry) {
    rtcReleaseGeometry(mGeometry);
    mGeometry = nullptr;
  }
}

void TriangulatedSphere::create(const glm::vec3& p, float r, int numPhi,
                                int numTheta) {
  mCenter = p;
  mRadius = r;

  RTCScene rtcScene = mOwner->getRTCScene();
  RTCDevice rtcDevice = rtcGetSceneDevice(rtcScene);

  /* create triangle mesh */
  RTCGeometry geom = rtcNewGeometry(rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE);
  mGeometry = geom;

  /* vertex and triangle layout */
  struct Vertex {
    float x, y, z, r;
  };
  struct Triangle {
    int v0, v1, v2;
  };

  /* map triangle and vertex buffers */
  Vertex* vertices = (Vertex*)rtcSetNewGeometryBuffer(
      geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, sizeof(Vertex),
      numTheta * (numPhi + 1));
  Triangle* triangles = (Triangle*)rtcSetNewGeometryBuffer(
      geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, sizeof(Triangle),
      2 * numTheta * (numPhi - 1));

  /* create sphere */
  float pi = glm::pi<float>();
  int tri = 0;
  const float rcpNumTheta = 1.0f / ((float)numTheta);
  const float rcpNumPhi = 1.0f / ((float)numPhi);
  for (int phi = 0; phi <= numPhi; phi++) {
    for (int theta = 0; theta < numTheta; theta++) {
      const float phif = phi * float(pi) * rcpNumPhi;
      const float thetaf = theta * 2.0f * float(pi) * rcpNumTheta;

      Vertex& v = vertices[phi * numTheta + theta];
      v.x = p.x + r * sin(phif) * sin(thetaf);
      v.y = p.y + r * cos(phif);
      v.z = p.z + r * sin(phif) * cos(thetaf);
    }
    if (phi == 0) continue;

    for (int theta = 1; theta <= numTheta; theta++) {
      int p00 = (phi - 1) * numTheta + theta - 1;
      int p01 = (phi - 1) * numTheta + theta % numTheta;
      int p10 = phi * numTheta + theta - 1;
      int p11 = phi * numTheta + theta % numTheta;

      if (phi > 1) {
        triangles[tri].v0 = p10;
        triangles[tri].v1 = p00;
        triangles[tri].v2 = p01;
        tri++;
      }

      if (phi < numPhi) {
        triangles[tri].v0 = p11;
        triangles[tri].v1 = p10;
        triangles[tri].v2 = p01;
        tri++;
      }
    }
  }

  rtcCommitGeometry(mGeometry);
  mRTCID = rtcAttachGeometry(rtcScene, mGeometry);
}

}  // namespace RTKit2