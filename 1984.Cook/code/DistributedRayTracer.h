/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <random>

#include "framework/MyRenderer.h"

namespace RayTracingHistory {

class DistributedRayTracer : public MyRenderer {
 public:
  DistributedRayTracer() : mDistribution(0.0f, 1.0f) {}

  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  glm::vec4 _sampleColor(float s, float t);

  float rand() { return mDistribution(mRandomEngine); }

 private:
  std::default_random_engine mRandomEngine;
  std::uniform_real_distribution<float> mDistribution;

  int mSPPn = 4;  // sample per pixel = n*n
};

}  // namespace RayTracingHistory
