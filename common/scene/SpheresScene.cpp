#include "SpheresScene.h"

#include "Sphere.h"
#include "Plane.h"

void SpheresScene::init() {
  createObject<Plane>("ground");
  createObject<Sphere>("sphereA").setCenter(glm::vec3(0, 1, 0)).setRadius(1.0f);
  createObject<Sphere>("sphereB").setCenter(glm::vec3(2, 0.5f, 0)).setRadius(0.5f);
}