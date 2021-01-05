#include "MySceneObject.h"

#include "MyScene.h"

namespace RTKit2 {

HitRecord MySceneObject::makeHitRecord(const MyRay& ray, unsigned int primID,
                                       float t, glm::vec2 uv, glm::vec3 N) {
  HitRecord hit;
  hit.isLight = false;
  hit.t = t;
  hit.pt = ray.getPoint(t);
  hit.uv = uv;
  hit.obj = this;
  hit.mtl = getMaterial();

  // unnormalized geometry normal in object space
  hit.normal = glm::normalize(N);
  return hit;
}

}  // namespace RTKit2