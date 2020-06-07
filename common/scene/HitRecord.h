/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>

namespace RayTracingHistory {

class MyMaterial;
class MySceneObject;

struct HitRecord {
  float t = 0;
  glm::vec3 p = {0, 0, 0};
  glm::vec3 normal = {0, 1, 0};
  glm::vec2 uv = {0, 0};
  MyMaterial* mtl = nullptr;
  MySceneObject* obj = nullptr;
};
}  // namespace RayTracingHistory
