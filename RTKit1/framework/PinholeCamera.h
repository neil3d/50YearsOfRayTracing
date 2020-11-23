/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "../geometry/Ray.h"
#include "MyCamera.h"

namespace RTKit1 {

class PinholeCamera : public MyCamera {
 public:
  Ray generateViewingRay(float u, float v) {
    glm::vec3 origin = mEyePos;
    return Ray(origin, mImagePlaneLeftTop + u * mImagePlaneH -
                           v * mImagePlaneV - origin);
  }

  void init() {
    constexpr float DIST = 1.0f;

    float halfHeight = tanf(mFov * 0.5f) * DIST;
    float halfWidth = mAspect * halfHeight;

    glm::vec3 center = mEyePos - mForward * DIST;
    mImagePlaneLeftTop = center - halfWidth * mRight + halfHeight * mUp;
    mImagePlaneH = 2 * halfWidth * mRight;
    mImagePlaneV = 2 * halfHeight * mUp;
  }

 private:
  glm::vec3 mImagePlaneH, mImagePlaneV, mImagePlaneLeftTop;
};
}  // namespace RTKit1
