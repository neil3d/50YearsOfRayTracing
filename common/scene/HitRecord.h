#pragma once
#include <glm/glm.hpp>

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