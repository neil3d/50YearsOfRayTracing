/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include <glm/glm.hpp>

namespace RTKit2 {

// Orthonormal Bases / Coordinate Frames
struct ONB {
  glm::vec3 U, V, W;

  ONB(glm::vec3 N) {
    W = glm::normalize(N);

    // choose any vector T not collinear with normal
    glm::vec3 T;
    if (fabsf(W.x) > 0.9f)
      T = glm::vec3(0, 1, 0);
    else
      T = glm::vec3(1, 0, 0);

    // constructing orthonormal bases
    U = glm::cross(T, W);
    V = glm::cross(W, U);
  }

  glm::vec3 localToWorld(const glm::vec3& p) {
    return p.x * U + p.y * V + p.z * W;
  }
};

}  // namespace RTKit1
