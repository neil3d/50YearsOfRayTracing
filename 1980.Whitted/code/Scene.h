#pragma once
#include <memory>

#include "Light.h"
#include "Material.h"
#include "scene/MyScene.h"
#include "scene/Parallelogram.h"
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
    constexpr float W = 8;
    constexpr float H = 14;
    createObject<Parallelogram>("Floor")
        .setParam(glm::vec3(W, 0, 0), glm::vec3(0, 0, H),
                  glm::vec3(-W / 2, 0, -H / 2))
        .createMaterial<Material>()
        .setCheckerTexture()
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    createObject<Sphere>("MirrorSphere")
        .setCenter(glm::vec3(0, 1, 1))
        .setRadius(1)
        .createMaterial<Material>()
        .setColor(glm::vec3(0, 1, 1))
        .setParam(0.3f, 0.7f, 0.0f, 120, 1.0f);

    createObject<Sphere>("GlassSphere")
        .setCenter(glm::vec3(1.25f, 2, -2))
        .setRadius(1.2f)
        .createMaterial<Material>()
        .setColor(glm::vec3(1))
        .setParam(0.0f, 0.0f, 0.7f, 60, 1.33f);

// create lights
#if 0
    auto pointLgt = std::make_shared<PointLight>(glm::vec3(-10, 10, -5));
    pointLgt->intensity = 0.85f;
    pointLgt->ambient = 0.15f;
    mLights.emplace_back(pointLgt);
#else
    auto dirLgt = std::make_shared<DirectionalLight>(glm::vec3(-2, -4, 2));
    dirLgt->intensity = 0.75f;
    dirLgt->ambient = 0.25f;
    mLights.emplace_back(dirLgt);
#endif
  }
};