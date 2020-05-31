/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include "framework/DirectRenderer.h"

namespace RayTracingHistory {

/**
 * A simple Monte Carlo solution for direct lighting only
 */ 
class DirectLightingRenderer : public DirectRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;
};

}  // namespace RayTracingHistory
