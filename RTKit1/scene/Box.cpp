#include "Box.h"

namespace RTKit1 {

bool Box::intersect(const Ray& ray, float tMin, float tMax, HitRecord& outRec) {
  Ray localRay = _makeLocalRay(ray, nullptr);

  bool hitAnySide = false;
  float closestSoFar = tMax;
  HitRecord rec, nearestRec;

  for (const auto& side : mSides) {
    if (side->intersect(localRay, tMin, closestSoFar, rec)) {
      hitAnySide = true;
      closestSoFar = rec.t;
      nearestRec = rec;
    }
  }  // end of for

  if (hitAnySide) {
    glm::vec4 HN =
        mTransform.getNormalMatrix() * glm::vec4(nearestRec.normal, 0);
    glm::vec3 N = glm::normalize(glm::vec3(HN));
    outRec = _makeHitRecord(ray, nearestRec.t, N, nearestRec.uv);
  }

  return hitAnySide;
}

void Box::init() {
  float W = mExtents.x;
  float H = mExtents.y;
  float D = mExtents.z;

  // top
  Parallelogram::Ptr top = std::make_shared<Parallelogram>("box.top");
  mSides[0] = top;
  top->setEdges(glm::vec3(0, 0, D), glm::vec3(W, 0, 0))
      .setAnchor(glm::vec3(W / -2, H, D / -2))
      .setMaterial(mMaterial);

  // bottom
  Parallelogram::Ptr bottom = std::make_shared<Parallelogram>("box.bottom");
  mSides[1] = bottom;
  bottom->setEdges(glm::vec3(W, 0, 0), glm::vec3(0, 0, D))
      .setAnchor(glm::vec3(W / -2, 0, D / -2))
      .setMaterial(mMaterial);

  // right
  Parallelogram::Ptr right = std::make_shared<Parallelogram>("box.right");
  mSides[2] = right;
  right->setEdges(glm::vec3(0, H, 0), glm::vec3(0, 0, D))
      .setAnchor(glm::vec3(W / 2, 0, D / -2))
      .setMaterial(mMaterial);

  // left
  Parallelogram::Ptr left = std::make_shared<Parallelogram>("box.left");
  mSides[3] = left;
  left->setEdges(glm::vec3(0, 0, D), glm::vec3(0, H, 0))
      .setAnchor(glm::vec3(W / -2, 0, D / -2))
      .setMaterial(mMaterial);

  Parallelogram::Ptr back = std::make_shared<Parallelogram>("box.back");
  mSides[4] = back;
  back->setEdges(glm::vec3(W, 0, 0), glm::vec3(0, H, 0))
      .setAnchor(glm::vec3(W / -2, 0, D / 2))
      .setMaterial(mMaterial);

  Parallelogram::Ptr front = std::make_shared<Parallelogram>("box.front");
  mSides[5] = front;
  front->setEdges(glm::vec3(0, H, 0), glm::vec3(W, 0, 0))
      .setAnchor(glm::vec3(W / -2, 0, -D / 2))
      .setMaterial(mMaterial);
}
}  // namespace RTKit1
