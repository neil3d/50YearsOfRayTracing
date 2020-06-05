/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <memory>

#include "DiffuseMaterial.h"
#include "LambertianMaterial.h"
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
    constexpr float H = 6;
    createObject<Parallelogram>("Floor")
        .setEdges(glm::vec3(0, 0, H), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, H / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(0.66f, 1, 0.36f));

    createObject<Sphere>("Sphere")
        .setCenter(glm::vec3(0, 1.f, 0))
        .setRadius(1)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1, 0.66f, 0.36f));

    createObject<Parallelogram>("Light")
        .setEdges(glm::vec3(0, 0, 2), glm::vec3(2, 0, 0))
        .setAnchor(glm::vec3(-3, 5, -1))
        .createMaterial<DiffuseMaterial>()
        .setEmission(1.0f);
  }
};
}  // namespace RayTracingHistory
