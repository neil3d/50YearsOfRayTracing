/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>

#include "scene/MyScene.h"
#include "scene/Parallelogram.h"
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class DemoScene : public MyScene {
 public:
  virtual void init() override {
    // create objects
    constexpr float W = 8;
    constexpr float H = 10;
    createObject<Parallelogram>("Floor")
        .setEdges(glm::vec3(0, 0, H), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, H / -2));

    createObject<Sphere>("Sphere").setCenter(glm::vec3(0, 2.f, 1)).setRadius(1);

    createObject<Parallelogram>("Light")
        .setEdges(glm::vec3(0, 0, 1), glm::vec3(1, 0, 0))
        .setAnchor(glm::vec3(-0.5f, 5, -0.5f));
  }
};
}  // namespace RayTracingHistory
