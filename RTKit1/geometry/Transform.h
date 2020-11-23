/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace RTKit1 {

static glm::mat4 identity = glm::identity<glm::mat4>();

struct Transform {
  void setPosition(const glm::vec3& pos) {
    position = pos;
    _update();
  }

  glm::vec3 getPosition() const { return position; }

  void setRotation(const glm::quat& rot) {
    rotation = rot;
    _update();
  }

  glm::quat getRotation() const { return rotation; }

  void setRotation(float pitch, float yaw, float roll) {
    rotation = glm::quat(glm::vec3(pitch, yaw, roll));
    _update();
  }

  void setScale(float s) {
    scale = glm::vec3(s);
    _update();
  }

  void setScale(const glm::vec3& s) {
    scale = s;
    _update();
  }

  const glm::mat4& getLocal2World() { return local2World; }

  const glm::mat4& getWorld2Local() { return world2Local; }

  const glm::mat4& getNormalMatrix() { return normalMatrix; }

 private:
  void _update() {
    auto matRotation = glm::mat4_cast(rotation);
    local2World = glm::translate(identity, position);
    local2World *= matRotation;
    local2World = glm::scale(local2World, scale);

    world2Local = glm::inverse(local2World);
    normalMatrix = glm::transpose(glm::inverse(local2World));
  }

  glm::vec3 position = {0, 0, 0};
  glm::quat rotation = {1, 0, 0, 0};
  glm::vec3 scale = {1, 1, 1};

  glm::mat4 local2World = identity;
  glm::mat4 world2Local = identity;
  glm::mat4 normalMatrix = identity;
};
}  // namespace RTKit1
