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
      getImagePlane(mImagePlaneH, mImagePlaneV, mImagePlaneLeftTop);
      mInited = true;
    }

    glm::vec3 origin = mEyePos;
    return Ray(origin, mImagePlaneLeftTop + u * mImagePlaneH -
                           v * mImagePlaneV - origin);
  }

 private:
  bool mInited = false;
  glm::vec3 mImagePlaneH, mImagePlaneV, mImagePlaneLeftTop;
};
}  // namespace RayTracingHistory
