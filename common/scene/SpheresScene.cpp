#include "SpheresScene.h"

#include "Sphere.h"

void SpheresScene::init() {
  auto& sphereA = createObject<Sphere>("sphereA");
  sphereA.setCenter(glm::vec3(0, 0, 1)).setRadius(2.0f);
}