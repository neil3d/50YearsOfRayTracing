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
  Ray generateViewingRay(float u, float v) {
    if (!mInited) {
      getFocalPlane(mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop);
      mInited = true;
    }

    glm::vec3 origin = mEyePos;
    return Ray(origin, mFocalPlaneLeftTop + u * mFocalPlaneH -
                           v * mFocalPlaneV - origin);
  }

 private:
  bool mInited = false;
  glm::vec3 mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop;
};
}  // namespace RayTracingHistory
