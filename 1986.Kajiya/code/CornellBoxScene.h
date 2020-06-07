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

namespace RayTracingHistory {

class CornellBoxScene : public MyScene {
 public:
  virtual void init() override {
    createObject<MeshInstance>("mesh").setMeshFile(
        "content/cornell_box/cornell_box.obj");
  }
};
}  // namespace RayTracingHistory
