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
    constexpr float W = 500;
    constexpr float D = 500;
    constexpr float H = 500;
    constexpr float LS = 3.5f;  // ligt size scale

    createObject<Parallelogram>("light")
        .setEdges(glm::vec3(0, 0, D / LS), glm::vec3(W / LS, 0, 0))
        .setAnchor(glm::vec3(W / -2 / LS, H - 0.01f, D / -2 / LS))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1))
        .setEmission(100.0f)
        .enableLight();

    createObject<Parallelogram>("ceiling")
        .setEdges(glm::vec3(W, 0, 0), glm::vec3(0, 0, D))
        .setAnchor(glm::vec3(W / -2, H, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    createObject<Parallelogram>("floor")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    createObject<Parallelogram>("left_wall")
        .setEdges(glm::vec3(0, H, 0), glm::vec3(0, 0, D))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(0, 1, 0));

    createObject<Parallelogram>("right_wall")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(0, H, 0))
        .setAnchor(glm::vec3(W / 2, 0, D / -2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1, 0, 0));

    createObject<Parallelogram>("back_wall")
        .setEdges(glm::vec3(0, H, 0), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / 2))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    float ball = W / 8;
    createObject<Sphere>("small_ball")
        .setCenter(glm::vec3(1.15f, ball * 4, 0))
        .setRadius(ball)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));

    ball *= 1.5f;
    createObject<Sphere>("big_ball")
        .setCenter(glm::vec3(-ball, ball, 0))
        .setRadius(ball)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));
  }
};
}  // namespace RayTracingHistory
