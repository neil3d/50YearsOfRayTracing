#include "TriangleMesh.h"

#include "MyScene.h"

namespace RTKit2 {

TriangleMesh::~TriangleMesh() {
  if (mGeometry) {
    rtcReleaseGeometry(mGeometry);
    mGeometry = nullptr;
  }
}

void TriangleMesh::createFromObj(WavefrontOBJ::Ptr model, int subMeshIndex,
                                 const Transform& trans) {
  mModel = model;
  mSubMeshIndex = subMeshIndex;
  mNormalMatrix = trans.getNormalMatrix();

  const auto& subMesh = model->getSubMeshes()[subMeshIndex];
  const auto& vertexBuffer = model->getVertexBuffer();
  int faceCount = subMesh.indices.size() / 3;

  // make geometry
  RTCScene rtcScene = mOwner->getRTCScene();
  RTCDevice rtcDevice = rtcGetSceneDevice(rtcScene);

  mGeometry = rtcNewGeometry(rtcDevice, RTC_GEOMETRY_TYPE_TRIANGLE);
  rtcSetSharedGeometryBuffer(mGeometry, RTC_BUFFER_TYPE_INDEX, 0,
                             RTC_FORMAT_UINT3, subMesh.indices.data(), 0,
                             3 * sizeof(unsigned int), faceCount);

#if 0  // TODO
  rtcSetSharedGeometryBuffer(mGeometry, RTC_BUFFER_TYPE_VERTEX, 0,
                             RTC_FORMAT_FLOAT3, vertexBuffer.data(), 0,
                             3 * sizeof(float), vertexBuffer.size());
#else
  // create & fill vertex buffer
  glm::vec4* vb = (glm::vec4*)rtcSetNewGeometryBuffer(
      mGeometry, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
      4 * sizeof(float), vertexBuffer.size());

  const auto& mat = trans.getLocal2World();
  for (size_t i = 0; i < vertexBuffer.size(); i++) {
    glm::vec4 vert(vertexBuffer[i], 1.0f);
    vb[i] = mat * vert;
  }
#endif

  // embree commit
  rtcCommitGeometry(mGeometry);
  mRTCID = rtcAttachGeometry(rtcScene, mGeometry);
}

HitRecord TriangleMesh::makeHitRecord(const MyRay& ray, unsigned int primID,
                                      float t, glm::vec2 uv, glm::vec3 N) {
  HitRecord hit = MySceneObject::makeHitRecord(ray, primID, t, uv, N);

  // handle per face material
  const auto& subMesh = mModel->getSubMeshes()[mSubMeshIndex];
  const auto& face = subMesh.faces[primID];
  hit.mtl = mMaterialBundle[face.materialID].get();

  // local to world
  glm::vec4 localNormal(mModel->getNormal(face, uv), 1.0f);
  glm::vec4 worldNormal = mNormalMatrix * localNormal;
  hit.normal = glm::vec3(worldNormal.x, worldNormal.y, worldNormal.z);
  return hit;
}

}  // namespace RTKit2