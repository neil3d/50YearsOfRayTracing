/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <tuple>

namespace RayTracingHistory {

namespace Triangle {

// 1997.TomasMoller
std::tuple<bool, float, glm::vec3> intersect(const Ray& ray,
                                             const glm::vec3& v0,
                                             const glm::vec3& v1,
                                             const glm::vec3& v2) {
  glm::vec3 dir = ray.direction;
  glm::vec3 orig = ray.origin;

  // find vectors for two edges sharing v0
  glm::vec3 edge1 = v1 - v0;
  glm::vec3 edge2 = v2 - v0;

  // begin calculating determinant - also used to calculate U parameter
  glm::vec3 pvec = glm::cross(dir, edge2);

  // if determinant is near zero, ray lies in plane of triangle
  float det = glm::dot(edge1, pvec);

  // enable culling
  if (det < glm::epsilon<float>())
    return std::make_tuple(false, 0, glm::vec3(0));

  // calculate distance from v0 to ray origin
  glm::vec3 tvec = orig - v0;

  // calculate U parameter and test bounds
  float u = glm::dot(tvec, pvec);
  if (u < 0.0f || u > det) return std::make_tuple(false, 0, glm::vec3(0));

  // prepare to test V parameter
  glm::vec3 qvec = glm::cross(tvec, edge1);

  // calculate V parameter and test bounds
  float v = glm::dot(dir, qvec);
  if (v < 0.0f || u + v > det) return std::make_tuple(false, 0, glm::vec3(0));

  // calculate t, scale parameters, ray intersects triangle
  float t = glm::dot(edge2, qvec);
  float invDet = 1.0f / det;

  t *= invDet;
  u *= invDet;
  v *= invDet;

  return std::make_tuple(true, t, glm::vec3(1.0f - u - v, u, v));
}

template <typename T>
T barycentricInterpolation(const glm::vec3& uvw, const T& v0, const T& v1,
                           const T& v2) {
  return uvw.x * v0 + uvw.y * v1 + uvw.z * v2;
}

}  // namespace Triangle

}  // namespace RayTracingHistory
