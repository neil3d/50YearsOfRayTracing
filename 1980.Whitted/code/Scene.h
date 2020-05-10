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
    // create objects
    createObject<Plane>("ground")
        .createMaterial<Material>()
        .setCheckerTexture()
        .setParam(0.6f, 0.0f, 0.0f, 60, 1.0f);

    createObject<Sphere>("MirrorSphere")
        .setCenter(glm::vec3(0, 1, 1))
        .setRadius(1)
        .createMaterial<Material>()
        .setColor(glm::vec3(0, 1, 1))
        .setParam(0.3f, 0.7f, 0.0f, 120, 1.0f);

    createObject<Sphere>("GlassSphere")
        .setCenter(glm::vec3(-1.25f, 2, -2))
        .setRadius(1.25f)
        .createMaterial<Material>()
        .setColor(glm::vec3(1))
        .setParam(0.0f, 0.0f, 0.7f, 60, 1.034f);

    // create lights
    auto pointLgt = std::make_shared<PointLight>(glm::vec3(-10, 10, -5));
    pointLgt->intensity = 1.5f;
    pointLgt->ambient = 0.2f;

    auto dirLgt = std::make_shared<DirectionalLight>(glm::vec3(-1, -5, 2));
    dirLgt->intensity = 0.5f;

    // mLights.emplace_back(pointLgt);
    mLights.emplace_back(dirLgt);
  }
};