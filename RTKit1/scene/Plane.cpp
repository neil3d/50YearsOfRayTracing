#include "Plane.h"

namespace RTKit1 {

bool Plane::intersect(const Ray& ray, float tMin, float tMax, HitRecord& outRec) {
  glm::vec3 pt = mTransform.getPosition();
  float denominator = glm::dot(mNormal, ray.direction);
  if (fabsf(denominator) > 0.0001f) {
    glm::vec3 difference = pt - ray.origin;
    float t = glm::dot(difference, mNormal) / denominator;
    if (t >= 0) {
      if (t > tMin && t < tMax) {
        outRec = _makeHitRecord(ray, t, mNormal, glm::vec2());
        outRec.uv = glm::vec2(outRec.p.x, outRec.p.z);
        return true;
      }
    }
  }  // end of if
  return false;
}
}  // namespace RTKit1
