/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <glm/gtc/random.hpp>

#include "TestSceneBase.h"
#include "scene/Plane.h"

namespace RayTracingHistory {

class TestScene : public TestSceneBase {
 public:
  virtual void init() override {
    TestSceneBase::init();

    // create a plane
    createObject<Plane>("floor")
        .createMaterial<Material>()
        .setColor(glm::vec3(0.66f))
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    // create spheres
    constexpr float D = 4;
    constexpr int COUNT = 3;

    for (int y = 0; y < COUNT; y++)
      for (int x = 0; x < COUNT; x++) {
        std::string szName("sphere_");
        szName.append(std::to_string(x + COUNT * y));

        float radius = glm::linearRand(0.25f, D / COUNT);
        glm::vec3 center;
        center.x = x * D - D / 2;
        center.z = y * D - D / 2;
        center.y = radius;

        _createSphere(szName, radius, center)
            .setColor(glm::vec3(1.0f))
            .setParam(0.7f, 0.3f, 0.0f, 80, 1.0f);
      }
  }
};
}  // namespace RayTracingHistory
