/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Light.h"
#include "Material.h"
#include "scene/MyScene.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class TestSceneBase : public MyScene {
 protected:
  std::vector<std::shared_ptr<MyLight>> mLights;

 public:
  const std::vector<std::shared_ptr<MyLight>>& getLights() const {
    return mLights;
  }

  virtual void init() override {
    // create lights
#if 0
    auto pointLgt = std::make_shared<PointLight>(glm::vec3(-10, 10, -5));
    pointLgt->intensity = 1.85f;
    pointLgt->ambient = 0.15f;
    mLights.emplace_back(pointLgt);
#else
    auto dirLgt = std::make_shared<DirectionalLight>(glm::vec3(2, -4, 2));
    dirLgt->intensity = 0.85f;
    dirLgt->ambient = 0.15f;
    mLights.emplace_back(dirLgt);
#endif
  }

 protected:
  Material& _createSphere(const std::string& szName, float radius,
                          glm::vec3 center) {
    auto& sphere =
        createObject<Sphere>(szName).setCenter(center).setRadius(radius);
    return sphere.createMaterial<Material>();
  }
};
}  // namespace RayTracingHistory