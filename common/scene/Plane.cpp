#include "Plane.h"

namespace RayTracingHistory {

bool Plane::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRec) {
  float denominator = glm::dot(mNormal, ray.direction);
  if (fabsf(denominator) > 0.0001f) {
    glm::vec3 difference = mP0 - ray.origin;
    float t = glm::dot(difference, mNormal) / denominator;
    if (t >= 0) {
      if (t > tMin && t < tMax) {
        outRec = _makeHitRecord(ray, t, mNormal, glm::vec2());
        return true;
      }
    }
  }  // end of if
  return false;
}
}  // namespace RayTracingHistory
