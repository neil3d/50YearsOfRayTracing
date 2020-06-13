/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include "AreaLight.h"
#include "scene/MyScene.h"

namespace RayTracingHistory {

class MySceneWithLight : public MyScene {
 public:
  virtual const AreaLight* getMainLight() const = 0;

  virtual float systemUnit() const { return 100; }
};
}  // namespace RayTracingHistory
