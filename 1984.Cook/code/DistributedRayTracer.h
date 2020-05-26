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

class MyScene;
class PinholeCamera;

class DistributedRayTracer : public MyRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  glm::vec4 _sampleColor(float s, float t, MyScene* pScene,
                         PinholeCamera* camera);

 private:
  int mSPPn = 2;  // sample per pixel = n*n
};

}  // namespace RayTracingHistory
