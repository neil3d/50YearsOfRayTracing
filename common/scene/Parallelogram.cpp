#include "Parallelogram.h"

bool Parallelogram::hit(const Ray& ray, float tMin, float tMax,
                        HitRecord& outRec) {
  glm::vec3 n = glm::vec3(this->plane);
  float dt = glm::dot(ray.direction, n);
  float t = (this->plane.w - glm::dot(n, ray.origin)) / dt;
  if (t > tMin && t < tMax) {
    glm::vec3 p = ray.origin + ray.direction * t;
    glm::vec3 vi = p - this->anchor;
    float a1 = glm::dot(this->v1, vi);
    if (a1 >= 0 && a1 <= 1) {
      float a2 = glm::dot(this->v2, vi);
      if (a2 >= 0 && a2 <= 1) {
        outRec.t = t;
        outRec.p = ray.getPoint(t);
        outRec.normal = n;
        outRec.obj = this;
        outRec.mtl = mMateril.get();
        return true;
      }
    }
  }

  return false;
}