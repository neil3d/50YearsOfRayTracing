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
    glm::vec3 origin = mEyePos;
    return Ray(origin, mImagePlaneLeftTop + u * mImagePlaneH -
                           v * mImagePlaneV - origin);
  }

  void init() {
    float halfHeight = tanf(mFov * 0.5f) * mZNear;
    float halfWidth = mAspect * halfHeight;

    glm::vec3 center = mEyePos + mForward * mZNear;
    mImagePlaneLeftTop = center - halfWidth * mRight + halfHeight * mUp;
    mImagePlaneH = 2 * halfWidth * mRight;
    mImagePlaneV = 2 * halfHeight * mUp;
  }

 private:
  float mZNear;
  glm::vec3 mImagePlaneH, mImagePlaneV, mImagePlaneLeftTop;
};
}  // namespace RayTracingHistory
