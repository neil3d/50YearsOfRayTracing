#pragma once
#include <glm/glm.hpp>
#include <memory>

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

  MyCamera& setPerspective(float fov, float zNear, float zFar) {
    mFov = glm::radians(fov);
    mZNear = zNear;
    mZFar = zFar;

    return *this;
  }

  MyCamera& setAspect(float aspect) {
    mAspect = aspect;
    return *this;
  }

  void getNearPlane(glm::vec3& outH, glm::vec3& outV,
                    glm::vec3& outLeftTop) const {
    float halfHeight = tanf(mFov * 0.5f) * mZNear;
    float halfWidth = mAspect * halfHeight;

    glm::vec3 center = mEyePos + mForward * mZNear;
    outLeftTop = center - halfWidth * mRight + halfHeight * mUp;
    outH = 2 * halfWidth * mRight;
    outV = 2 * halfHeight * mUp;
  }

  glm::vec3 getEyePos() const { return mEyePos; }

 private:
  glm::vec3 mEyePos;
  glm::vec3 mForward, mRight, mUp;
  float mFov, mZNear, mZFar;
  float mAspect;
};