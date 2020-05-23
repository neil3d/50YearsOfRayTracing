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
    float temp = (-b - sqrt(disc)) / a;

    if (temp < closestSoFar && temp > tMin) {
      closestSoFar = temp;
      glm::vec3 normal = -glm::normalize(ray.getPoint(temp) - center);
      outRec = _makeHitRecord(ray, temp, normal, glm::vec2());
      hit = true;
    }

    temp = (-b + sqrt(disc)) / a;
    if (temp < closestSoFar && temp > tMin) {
      closestSoFar = temp;
      glm::vec3 normal = glm::normalize(ray.getPoint(temp) - center);
      outRec = _makeHitRecord(ray, temp, normal, glm::vec2());
      hit = true;
    }
  }

  return hit;
}