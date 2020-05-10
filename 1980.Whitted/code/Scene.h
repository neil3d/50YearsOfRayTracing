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
    createObject<Plane>("ground").createMaterial<Material>().setColor(
        glm::vec3(c, 0, 0));

    createObject<Sphere>("sphereA")
        .setCenter(glm::vec3(0, 0.5, 0))
        .setRadius(0.5f)
        .createMaterial<Material>()
        .setColor(glm::vec3(0, c, 0));

    createObject<Sphere>("sphereB")
        .setCenter(glm::vec3(1, 0.5f, 0))
        .setRadius(0.5f)
        .createMaterial<Material>()
        .setColor(glm::vec3(0, 0, c));
    ;

    mLights.emplace_back(std::make_shared<PointLight>(glm::vec3(-10, 10, 0)));
    mLights.emplace_back(std::make_shared<PointLight>(glm::vec3(10, 20, 0)));
  }
};