#include "Sphere.h"

namespace RayTracingHistory {

static bool _solveQuadratic(float a, float b, float c, float &x0, float &x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0.f) return false;

  x0 = 0.5f * (-b + sqrtf(discr)) / a;
  x1 = 0.5f * (-b - sqrtf(discr)) / a;
  return true;
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &outRec) {
  glm::vec3 center = getCenter();
  // analytic solution
  glm::vec3 oc = ray.origin - center;
  float a = glm::dot(ray.direction, ray.direction);
  float b = 2 * glm::dot(oc, ray.direction);
  float c = glm::dot(oc, oc) - mRadius * mRadius;

  float t0, t1;
  if (!_solveQuadratic(a, b, c, t0, t1)) return false;
  float tnear = std::min(t0, t1);

  if (tnear > tMin && tnear < tMax) {
    glm::vec3 N = glm::normalize(ray.getPoint(tnear) - center);
    float u = (1 + atan2(N.z, N.x) / M_PI) * 0.5f;
    float v = acosf(N.y) / M_PI;

    outRec = _makeHitRecord(ray, tnear, N, glm::vec2(u, v));
    return true;
  }

  return false;
}

}  // namespace RayTracingHistory
