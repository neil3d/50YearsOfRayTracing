/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once

#include "AreaLight.h"
#include "scene/MyScene.h"

namespace RTKit1 {

class MySceneWithLights : public MyScene {
 protected:
  std::vector<std::shared_ptr<AreaLight>> mLights;

 public:
  const std::vector<std::shared_ptr<AreaLight>>& getLights() const {
    return mLights;
  }

  virtual float systemUnit() const { return 100; }
};
}  // namespace RTKit1
