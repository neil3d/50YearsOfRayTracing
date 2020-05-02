#include "Sphere.h"

bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRec) {
  glm::vec3 center = mCenter;  // world space center position
  float radius = mRadius;

  glm::vec3 oc = ray.origin - center;
  float a = glm::dot(ray.direction, ray.direction);
  float b = glm::dot(oc, ray.direction);
  float c = glm::dot(oc, oc) - radius * radius;

  bool hit = false;
  float closestSoFar = tMax;

  float disc = b * b - a * c;
  if (disc > 0) {
    float temp = (-b - sqrt(b * b - a * c)) / a;

    if (temp < closestSoFar && temp > tMin) {
      closestSoFar = temp;
      outRec.t = temp;
      outRec.p = ray.getPoint(temp);
      outRec.normal = (outRec.p - center) / radius;
      outRec.obj = this;
      outRec.mtl = mMateril.get();
      hit = true;
    }

    temp = (-b + sqrt(b * b - a * c)) / a;
    if (temp < closestSoFar && temp > tMin) {
      closestSoFar = temp;
      outRec.t = temp;
      outRec.p = ray.getPoint(temp);
      outRec.normal = (outRec.p - center) / radius;
      outRec.obj = this;
      outRec.mtl = mMateril.get();
      hit = true;
    }
  }

  return hit;
}