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

class ThinLensCamera : public MyCamera {
 public:
  ThinLensCamera& setAperture(float val) {
    mAperture = val;
    return *this;
  }

  ThinLensCamera& setFocusDist(float val) {
    mFocusDist = val;
    return *this;
  }

  Ray jitteredViewingRay(float u, float v, const glm::vec2& xi) const {
    glm::vec3 offset = mAperture * xi.x * mRight + mAperture * xi.y * mUp;
    glm::vec3 origin = mEyePos + offset;
    glm::vec3 focus = mFocalPlaneLeftTop + u * mFocalPlaneH - v * mFocalPlaneV;
    return Ray(origin, focus - origin);
  }

  void init() {
    float halfHeight = tanf(mFov * 0.5f) * mFocusDist;
    float halfWidth = mAspect * halfHeight;

    glm::vec3 center = mEyePos + mForward * mFocusDist;
    mFocalPlaneLeftTop = center - halfWidth * mRight + halfHeight * mUp;
    mFocalPlaneH = 2 * halfWidth * mRight;
    mFocalPlaneV = 2 * halfHeight * mUp;
  }

 private:
  float mAperture = 0.001f;
  float mFocusDist = 4.0f;
  glm::vec3 mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop;
};

}  // namespace RayTracingHistory
