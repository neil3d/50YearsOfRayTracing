/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
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

namespace RTKit1 {

class TestSceneBase : public MyScene {
 protected:
  std::vector<std::shared_ptr<MyLight>> mLights;

 public:
  const std::vector<std::shared_ptr<MyLight>>& getLights() const {
    return mLights;
  }

 protected:
  void _createLights(bool dirLight, bool pointLight) {
    float s = 1.0f / ((int)dirLight + pointLight);
    if (dirLight) {
      auto dirLgt = std::make_shared<DirectionalLight>(glm::vec3(2, -4, 2));
      dirLgt->intensity = 0.85f * s;
      dirLgt->ambient = 0.15f * s;
      mLights.emplace_back(dirLgt);
    }

    if (pointLight) {
      auto pointLgt = std::make_shared<PointLight>(glm::vec3(2.5f, 5.f, 0));
      pointLgt->intensity = 1 * s;
      pointLgt->ambient = 0.15f * s;
      mLights.emplace_back(pointLgt);
    }
  }

  Material& _createSphere(const std::string& szName, float radius,
                          glm::vec3 center) {
    auto& sphere =
        createObject<Sphere>(szName).setCenter(center).setRadius(radius);
    return sphere.createMaterial<Material>();
  }
};
}  // namespace RTKit1
