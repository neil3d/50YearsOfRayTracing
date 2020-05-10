#pragma once
#include <memory>

#include "Light.h"
#include "Material.h"
#include "scene/MyScene.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

class Scene : public MyScene {
  std::vector<std::shared_ptr<MyLight>> mLights;

 public:
  const std::vector<std::shared_ptr<MyLight>>& getLights() const {
    return mLights;
  }

  virtual void init() override {
    float c = 0.7f;

    // create objects
    createObject<Plane>("ground")
        .createMaterial<Material>()
        .setCheckerTexture();

    createObject<Sphere>("sphereA")
        .setCenter(glm::vec3(0, 1, 0))
        .setRadius(1)
        .createMaterial<Material>()
        .setColor(glm::vec3(0, c, 0));

    createObject<Sphere>("sphereB")
        .setCenter(glm::vec3(2, 1, 0))
        .setRadius(0.5f)
        .createMaterial<Material>()
        .setColor(glm::vec3(0, 0, c));
    ;

    // create lights
    auto pointLgt = std::make_shared<PointLight>(glm::vec3(-10, 10, 0));
    pointLgt->intensity = 1.5f;

    auto dirLgt = std::make_shared<DirectionalLight>(glm::vec3(-1, -5, 2));
    dirLgt->intensity = 0.5f;

    mLights.emplace_back(pointLgt);
    mLights.emplace_back(dirLgt);
  }
};