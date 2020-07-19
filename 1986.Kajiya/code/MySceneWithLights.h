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

class MySceneWithLights : public MyScene {
 protected:
  std::vector<std::shared_ptr<AreaLight>> mLights;

 public:
  const std::vector<std::shared_ptr<AreaLight>>& getLights() const {
    return mLights;
  }

  virtual float systemUnit() const { return 100; }
};
}  // namespace RayTracingHistory
