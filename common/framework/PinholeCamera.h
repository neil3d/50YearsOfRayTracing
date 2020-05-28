/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "../geometry/Ray.h"
#include "MyCamera.h"

namespace RayTracingHistory {

class PinholeCamera : public MyCamera {
 public:
  MyCamera& setZNear(float d) {
    mZNear = d;
    return *this;
  }

  Ray generateViewingRay(float u, float v) {
    if (!mInited) {
      _getImagePlane(mImagePlaneH, mImagePlaneV, mImagePlaneLeftTop);
      mInited = true;
    }

    glm::vec3 origin = mEyePos;
    return Ray(origin, mImagePlaneLeftTop + u * mImagePlaneH -
                           v * mImagePlaneV - origin);
  }

 private:
  void _getImagePlane(glm::vec3& outH, glm::vec3& outV,
                      glm::vec3& outLeftTop) const {
    float halfHeight = tanf(mFov * 0.5f) * mZNear;
    float halfWidth = mAspect * halfHeight;

    glm::vec3 center = mEyePos + mForward * mZNear;
    outLeftTop = center - halfWidth * mRight + halfHeight * mUp;
    outH = 2 * halfWidth * mRight;
    outV = 2 * halfHeight * mUp;
  }

 private:
  bool mInited = false;
  float mZNear;
  glm::vec3 mImagePlaneH, mImagePlaneV, mImagePlaneLeftTop;
};
}  // namespace RayTracingHistory
