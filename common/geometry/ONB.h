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

struct ONB {
  glm::vec3 U, V, W;

  ONB(glm::vec3 N) {
    N = glm::normalize(N);

    // choose any vector T not collinear with normal
    glm::vec3 T = N;
    if (T.x < T.y && T.x < T.z) {
      T.x = 1.0f;
    } else if (T.y < T.x && T.y < T.z) {
      T.y = 1.0f;
    } else
      T.z = 1.0f;

    T = glm::normalize(T);

    // constructing orthonormal bases
    U = glm::cross(T, N);
    V = glm::cross(N, U);
    W = N;
  }

  glm::vec3 local(const glm::vec3& p) { return p.x * U + p.y * V + p.z * W; }
};

}  // namespace RayTracingHistory
