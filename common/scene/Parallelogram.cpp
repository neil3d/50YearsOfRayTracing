#include "Parallelogram.h"

namespace RayTracingHistory {

bool Parallelogram::hit(const Ray& ray, float tMin, float tMax,
                        HitRecord& outRec) {
  glm::vec3 anchor = getAnchor();
  HitRecord pHit;
  if (Plane::hit(ray, tMin, tMax, pHit)) {
    // project Pt to edge
    glm::vec3 vi = pHit.p - anchor;
    float a1 = glm::dot(this->edge1, vi) / len1;
    if (a1 >= 0 && a1 <= len1) {
      float a2 = glm::dot(this->edge2, vi) / len2;
      if (a2 >= 0 && a2 <= len2) {
        outRec = pHit;
        return true;
      }
    }
  }

  return false;
}

}  // namespace RayTracingHistory
