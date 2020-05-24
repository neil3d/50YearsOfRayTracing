/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */
#pragma once
#include "framework/MyRenderer.h"

namespace RayTracingHistory {

class RayCastingRenderer : public MyRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  Ray _generateViewRay(int x, int y);
  Ray _generateShadowRay(const glm::vec3& point);

  void _drawSinglePixel(int x, int y, MyScene* pScene);

  // light
  glm::vec3 mLightPos = {-10, 10, 0};

  // cache camera frame
  glm::vec3 mEyePos;
  glm::vec3 mFocalPlaneH, mFocalPlaneV, mFocalPlaneLeftTop;
};

}  // namespace RayTracingHistory
