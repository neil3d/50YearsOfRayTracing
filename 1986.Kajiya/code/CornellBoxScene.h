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
#include "scene/MeshInstance.h"
#include "scene/MyScene.h"
#include "scene/Parallelogram.h"

namespace RayTracingHistory {

class CornellBoxScene : public MyScene {
 public:
  virtual void init() override {
    constexpr float W = 556;
    constexpr float D = 556;
    constexpr float H = 548;
    constexpr float LS = 3.5f;  // ligt size scale

    createObject<Parallelogram>("light")
        .setEdges(glm::vec3(0, 0, D / LS), glm::vec3(W / LS, 0, 0))
        .setAnchor(glm::vec3(213, H - 0.01f, 227))
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1))
        .setEmission(100.0f)
        .enableLight();

    const char* const szFileName = "content/cornell_box/cornell_box.obj";
    // const char* const szFileName = "content/tank/tank.obj";
    createObject<MeshInstance>("mesh")
        .setMeshFile(szFileName)
        .createMaterial<DiffuseMaterial>()
        .setColor(glm::vec3(1));
    ;
  }
};
}  // namespace RayTracingHistory
