/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace RayTracingHistory {

struct Transform {
  void setPosition(const glm::vec3& pos) {
    position = pos;
    bDirty = true;
  }

  glm::vec3 getPosition() const { return position; }

  void setRotation(const glm::quat& rot) {
    rotation = rot;
    bDirty = true;
  }

  glm::quat getRotation() const { return rotation; }

  void setRotation(float pitch, float yaw, float roll) {
    rotation = glm::quat(glm::vec3(pitch, yaw, roll));
    bDirty = true;
  }

  void setScale(float s) {
    scale = glm::vec3(s);
    bDirty = true;
  }

  void setScale(const glm::vec3& s) {
    scale = s;
    bDirty = true;
  }

  const glm::mat4& getLocal2World() {
    _update();
    return local2World;
  }

  const glm::mat4& getWorld2Local() {
    _update();
    return world2Local;
  }

  const glm::mat4& getWorld2LocalT() {
    _update();
    return world2LocalT;
  }

 private:
  void _update() {
    if (!bDirty) return;
    bDirty = false;

    static glm::mat4 identity = glm::identity<glm::mat4>();
    local2World = glm::translate(identity, position);
    local2World *= glm::mat4_cast(rotation);
    local2World = glm::scale(local2World, scale);

    world2Local = glm::inverse(local2World);
    world2LocalT = glm::transpose(world2Local);
  }

  glm::vec3 position = {0, 0, 0};
  glm::quat rotation = {1, 0, 0, 0};
  glm::vec3 scale = {1, 1, 1};

  bool bDirty = true;
  glm::mat4 local2World;
  glm::mat4 world2Local;
  glm::mat4 world2LocalT;  // for normal transformation
};
}  // namespace RayTracingHistory
