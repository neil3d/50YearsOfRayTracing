/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */
#pragma once
#include <memory>
#include <vector>

#include "TestSceneBase.h"
#include "scene/MeshInstance.h"
#include "scene/Parallelogram.h"
#include "scene/Plane.h"

namespace RayTracingHistory {

class DemoScene : public TestSceneBase {
 public:
  virtual void init() override {
    _createLights(true, false);

    // create objects
    constexpr float W = 8;
    constexpr float H = 16;
    createObject<Parallelogram>("Floor")
        .setEdges(glm::vec3(0, 0, H), glm::vec3(W, 0, 0))
        .setAnchor(glm::vec3(W / -2, 0, H / -2))
        .createMaterial<Material>()
        .setCheckerTexture()
        .setParam(1.0f, 0.0f, 0.0f, 60, 1.0f);

    _createSphere("MirrorSphere", 1, glm::vec3(0, 1.f, 1))
        .setColor(glm::vec3(0.4f, 0.9f, 1.0f))
        .setParam(0.7f, 0.3f, 0.0f, 100, 1.0f);

#if 0
    _createSphere("MirrorSphere", 1, glm::vec3(2.1f, 1.f, 1))
        .setColor(glm::vec3(0.4f, 0.9f, 1.0f))
        .setParam(0.7f, 0.3f, 0.0f, 100, 1.0f);

    _createSphere("MirrorSphere", 1.45f, glm::vec3(-3.f, 1.45f, 1))
        .setColor(glm::vec3(0.4f, 0.9f, 1.0f))
        .setParam(0.7f, 0.3f, 0.0f, 100, 1.0f);
#endif

    // using negative radius
    _createSphere("GlassSphere", -1.0f, glm::vec3(-1.2f, 1.65f, -2.0f))
        .setColor(glm::vec3(1))
        .setParam(0.0f, 0.2f, 0.8f, 80, 1.05f);
  }
};

}  // namespace RayTracingHistory
