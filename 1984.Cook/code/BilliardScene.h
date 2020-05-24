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
#include "scene/Plane.h"
#include "scene/Sphere.h"

namespace RayTracingHistory {

class BilliardScene : public MyScene {
 public:
  virtual void init() override {}
};
}  // namespace RayTracingHistory
