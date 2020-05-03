#include "SpheresScene.h"

#include "Sphere.h"
#include "Plane.h"

void SpheresScene::init() {
  auto& ground = createObject<Plane>("ground");

  auto& sphereA = createObject<Sphere>("sphereA");
  sphereA.setCenter(glm::vec3(0, 1, 0)).setRadius(1.0f);
}