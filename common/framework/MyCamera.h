/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace RayTracingHistory {

class MyCamera {
 public:
  typedef std::shared_ptr<MyCamera> Ptr;

  MyCamera& setView(glm::vec3 eyePos, glm::vec3 lookAt, glm::vec3 up) {
    mEyePos = eyePos;

    mForward = glm::normalize(lookAt - eyePos);
    mRight = glm::normalize(glm::cross(up, mForward));
    mUp = glm::cross(mForward, mRight);

    return *this;
  }

  MyCamera& setFOV(float fovInDegree) {
    mFov = glm::radians(fovInDegree);
    return *this;
  }

  MyCamera& setFocalLength(float d) {
    mFocalLength = d;
    return *this;
  }

  MyCamera& setAspect(float aspect) {
    mAspect = aspect;
    return *this;
  }

  void getImagePlane(glm::vec3& outH, glm::vec3& outV,
                     glm::vec3& outLeftTop) const {
    float halfHeight = tanf(mFov * 0.5f) * mFocalLength;
    float halfWidth = mAspect * halfHeight;

    glm::vec3 center = mEyePos + mForward * mFocalLength;
    outLeftTop = center - halfWidth * mRight + halfHeight * mUp;
    outH = 2 * halfWidth * mRight;
    outV = 2 * halfHeight * mUp;
  }

  glm::vec3 getEyePos() const { return mEyePos; }

 protected:
  glm::vec3 mEyePos;
  glm::vec3 mForward, mRight, mUp;
  float mFov, mFocalLength;
  float mAspect;
};
}  // namespace RayTracingHistory
