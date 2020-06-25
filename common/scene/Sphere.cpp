#include "Sphere.h"

#include <algorithm>
#include <glm/gtc/constants.hpp>

namespace RayTracingHistory {

static bool _solveQuadratic(float a, float b, float c, float &x0, float &x1) {
  float discr = b * b - 4 * a * c;
  if (discr < 0.f) return false;

  x0 = 0.5f * (-b + sqrtf(discr)) / a;
  x1 = 0.5f * (-b - sqrtf(discr)) / a;
  return true;
}

bool Sphere::intersect(const Ray &ray, float tMin, float tMax,
                       HitRecord &outRec) {
  Transform transform = mTransform;
  if (mAnimator) mAnimator->evaluate(ray.time, &transform);

  // intersection in local space
  Ray localRay = _makeLocalRay(ray, &transform);
  glm::vec3 center(0, 0, 0);

  // analytic solution
  glm::vec3 oc = localRay.origin - center;
  float a = glm::dot(localRay.direction, localRay.direction);
  float b = 2 * glm::dot(oc, localRay.direction);
  float c = glm::dot(oc, oc) - mRadius * mRadius;

  float t0, t1;
  if (!_solveQuadratic(a, b, c, t0, t1)) return false;

  float tnear = std::min(t0, t1);

  if (t0 * t1 < 0) {
    if (t0 > 0)
      tnear = t0;
    else
      tnear = t1;
  }

  if (tnear > tMin && tnear < tMax) {
    constexpr float PI = glm::pi<float>();

    glm::vec3 localN = glm::normalize(localRay.getPoint(tnear) - center);
    glm::vec3 N(transform.getNormalMatrix() * glm::vec4(localN, 0));
    float u = (1 + atan2(localN.z, localN.x) / PI) * 0.5f;
    float v = acosf(localN.y) / PI;

    outRec = _makeHitRecord(ray, tnear, N, glm::vec2(u, v));
    return true;
  }

  return false;
}

}  // namespace RayTracingHistory
