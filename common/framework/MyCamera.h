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

    mForward = glm::normalize(eyePos - lookAt);
    mRight = glm::normalize(glm::cross(up, mForward));
    mUp = glm::cross(mForward, mRight);

    return *this;
  }

  MyCamera& setFOV(float fovInDegree) {
    mFov = glm::radians(fovInDegree);
    return *this;
  }

  MyCamera& setAspect(float aspect) {
    mAspect = aspect;
    return *this;
  }

  glm::vec3 getEyePos() const { return mEyePos; }

 protected:
  glm::vec3 mEyePos;
  glm::vec3 mForward, mRight, mUp;
  float mFov = 60.0f;
  float mAspect = 1.0f;
};
}  // namespace RayTracingHistory
