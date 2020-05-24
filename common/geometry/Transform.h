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

  void setRotation(const glm::quat& rot) {
    rotation = rot;
    bDirty = true;
  }

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

 private:
  void _update() {
    if (!bDirty) return;
    bDirty = false;

    mat = glm::mat4_cast(rotation);
    mat[0][3] = position.x;
    mat[1][3] = position.y;
    mat[2][3] = position.z;
    mat = glm::scale(mat, scale);

    invMat = glm::inverse(mat);
  }

  glm::vec3 position;
  glm::quat rotation;
  glm::vec3 scale;

  bool bDirty = true;
  glm::mat4 mat;
  glm::mat4 invMat;
};
}  // namespace RayTracingHistory
