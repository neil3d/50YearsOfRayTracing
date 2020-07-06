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
#include "scene/Parallelogram.h"

namespace RayTracingHistory {

class BoxScene : public TestSceneBase {
 public:
  virtual void init() override {
    _createLights(false, true);

    constexpr float W = 8;
    constexpr float D = 8;
    constexpr float H = 8;

    createObject<Parallelogram>("floor")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<Material>()
        .setColor(glm::vec3(1))
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    createObject<Parallelogram>("left_wall")
        .setEdges(glm::vec3(0, H, 0), glm::vec3(0, 0, D))
        .setAnchor(glm::vec3(W / -2, 0, D / -2))
        .createMaterial<Material>()
        .setColor(glm::vec3(0, 1, 0))
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    createObject<Parallelogram>("right_wall")
        .setEdges(glm::vec3(0, 0, D), glm::vec3(0, H, 0))
        .setAnchor(glm::vec3(W / 2, 0, D / -2))
        .createMaterial<Material>()
        .setColor(glm::vec3(1, 0, 0))
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    createObject<Parallelogram>("ceiling")
        .setEdges(glm::vec3(W, 0, 0), glm::vec3(0, 0, D))
        .setAnchor(glm::vec3(W / -2, H, D / -2))
        .createMaterial<Material>()
        .setColor(glm::vec3(1))
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    createObject<Parallelogram>("back_wall")
        .setEdges(glm::vec3(0, H, 0), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, D / 2))
        .createMaterial<Material>()
        .setColor(glm::vec3(1))
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    _createSphere("GlassSphere", 1.25f, glm::vec3(0, 1.5f, 0))
        .setColor(glm::vec3(1))
        .setParam(0.2f, 0.2f, 0.8f, 80, 1.5f);
  }
};
}  // namespace RayTracingHistory
