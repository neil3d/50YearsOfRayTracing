#include "Mesh.h"
#include <tiny_obj_loader.h>

Mesh& Mesh::loadWavefrontObj(const std::string& szFileName) { return *this; }

Mesh& Mesh::setLocation(glm::vec3 loc) { return *this; }

Mesh& Mesh::setScale(glm::vec3 scale) { return *this; }

Mesh& Mesh::setRotation(float pitch, float yaw, float roll) { return *this; }

bool Mesh::hit(const Ray& ray, float tMin, float tMax, HitRecord& outRec) {
  return false;
}