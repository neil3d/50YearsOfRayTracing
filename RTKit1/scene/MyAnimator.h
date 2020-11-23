/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "../geometry/Transform.h"

namespace RTKit1 {

struct PosKey {
  float t;
  glm::vec3 pos;
  PosKey(float inT, const glm::vec3& inPos) : t(inT), pos(inPos) {}
};

struct RotKey {
  float t;
  glm::quat rot;
  RotKey(float inT, const glm::quat& inRot) : t(inT), rot(inRot) {}
};

class MyAnimator {
  std::vector<PosKey> mPosTrack;
  std::vector<RotKey> mRotTrack;

 public:
  typedef std::shared_ptr<MyAnimator> Ptr;
  MyAnimator() = default;

  void addKey(float t, const Transform& trans) {
    mPosTrack.emplace_back(t, trans.getPosition());
    mRotTrack.emplace_back(t, trans.getRotation());
  }

  void addKey(float t, const glm::vec3& pos, const glm::quat& rot) {
    mPosTrack.emplace_back(t, pos);
    mRotTrack.emplace_back(t, rot);
  }

  void addKey(float t, const glm::vec3& pos, const glm::vec3& radAngles) {
    mPosTrack.emplace_back(t, pos);
    mRotTrack.emplace_back(t, glm::quat(radAngles));
  }

  void evaluate(float t, Transform* target) {
    if (mPosTrack.size() < 2) return;
    if (mRotTrack.size() < 2) return;

    // TODO: full impl
    int k1 = 0;
    int k2 = 1;
    float duration = 1.0f;
    float r = t / duration;

    const auto& posK1 = mPosTrack[k1];
    const auto& posK2 = mPosTrack[k2];
    target->setPosition((1 - r) * posK1.pos + r * posK2.pos);

    const auto& rotKey1 = mRotTrack[k1];
    const auto& rotKey2 = mRotTrack[k2];
    glm::quat rot = glm::lerp(rotKey1.rot, rotKey2.rot, r);
    target->setRotation(rot);
  }
};

}  // namespace RTKit1
