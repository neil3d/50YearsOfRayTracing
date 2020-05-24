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
  glm::vec3 p;
  glm::vec3 normal;
  glm::vec2 uv;
  MyMaterial* mtl = nullptr;
  MySceneObject* obj = nullptr;
};
}  // namespace RayTracingHistory
