#pragma once
#include <glm/glm.hpp>
#include <memory>

class MyCamera {
 public:
  typedef std::shared_ptr<MyCamera> Ptr;

  MyCamera& setView(glm::vec3 eyePos, glm::vec3 lookAt, glm::vec3 up) {
    mEyePos = eyePos;

    mForward = glm::normalize(eyePos - lookAt);
    mSide = glm::normalize(glm::cross(up, eyePos));
    mUp = glm::cross(mForward, mSide);

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
                    glm::vec3& outLeftBottom) const {
    float halfHeight = tanf(mFov * 0.5f);
    float halfWidth = mAspect * halfHeight;

    outLeftBottom = mEyePos - halfWidth * mZNear * mSide -
                    halfHeight * mZNear * mUp - mForward * mZNear;
    outH = 2 * halfWidth * mZNear * mSide;
    outV = 2 * halfHeight * mZNear * mUp;
  }

  glm::vec3 getEyePos() const { return mEyePos; }
  glm::vec3 getSide() const { return mSide; }
  glm::vec3 getUp() const { return mUp; }

 private:
  glm::vec3 mEyePos;
  glm::vec3 mForward, mSide, mUp;
  float mFov, mZNear, mZFar;
  float mAspect;
};