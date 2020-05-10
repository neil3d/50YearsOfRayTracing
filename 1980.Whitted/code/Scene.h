#pragma once

#include "Material.h"
#include "scene/MyScene.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

class Scene : public MyScene {
 public:
  virtual void init() override {
    createObject<Plane>("ground");

    createObject<Sphere>("sphereA")
        .setCenter(glm::vec3(0, 1, 0))
        .setRadius(1.0f)
        .createMaterial<Material>();

    createObject<Sphere>("sphereB")
        .setCenter(glm::vec3(2, 0.5f, 0))
        .setRadius(0.5f);
  }
};