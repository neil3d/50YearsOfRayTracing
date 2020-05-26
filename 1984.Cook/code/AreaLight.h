/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

struct AreaLight {
  glm::vec3 edge1, edge2;
  glm::vec3 corner;

  float ambient = 0.1f;
  float intensity = 2.0f;

  AreaLight& setParallelogram(const glm::vec3& _edge1, const glm::vec3& _edge2,
                              const glm::vec3& _corner) {
    edge1 = _edge1;
    edge2 = _edge2;
    corner = _corner;
  }

  glm::vec3 getSamplePoint() {
    glm::vec2 rand = glm::diskRand(1.0f);
    return glm::vec3();
  }
};