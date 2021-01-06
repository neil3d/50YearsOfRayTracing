#include "TriangleMesh.h"

#include "MyScene.h"

namespace RTKit2 {

TriangleMesh::~TriangleMesh() {
  if (mGeometry) {
    rtcReleaseGeometry(mGeometry);
    mGeometry = nullptr;
  }
}

void TriangleMesh::createFromObj(const std::string& assetKey,
                                const Transform& trans) {}

}  // namespace RTKit2