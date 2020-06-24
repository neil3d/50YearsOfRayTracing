/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "scene/MyScene.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class DemoScene : public MyScene {
 public:
  virtual void init() override {
    createObject<Plane>("ground");

    createObject<Sphere>("sphereA")
        .setCenter(glm::vec3(0, 1, 0))
        .setRadius(1.0f);

    createObject<Sphere>("sphereB")
        .setCenter(glm::vec3(-2, 0.5f, 0))
        .setRadius(0.5f);
  }
};

}  // namespace RayTracingHistory
