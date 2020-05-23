#include "Sphere.h"

static bool _solveQuadratic(float a, float b, float c, float &x0, float &x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0.f) return false;

  if (fabsf(discr) < 0.0001f)
    x0 = x1 = -0.5f * b / a;
  else {
    x0 = 0.5f * (-b + sqrtf(discr)) / a;
    x1 = 0.5f * (-b - sqrtf(discr)) / a;
  }
  return true;
}

bool Sphere::hit(const Ray &ray, float tMin, float tMax, HitRecord &outRec) {
  // analytic solution
  glm::vec3 oc = ray.origin - mCenter;
  float a = glm::dot(ray.direction, ray.direction);
  float b = 2 * glm::dot(oc, ray.direction);
  float c = glm::dot(oc, oc) - mRadius * mRadius;

  float t0, t1;
  if (!_solveQuadratic(a, b, c, t0, t1)) return false;
  float tnear = std::min(t0, t1);

  if (tnear > tMin && tnear < tMax) {
    glm::vec3 normal = glm::normalize(ray.getPoint(tnear) - mCenter);
    outRec = _makeHitRecord(ray, tnear, normal, glm::vec2());
    return true;
  }

  return false;
}