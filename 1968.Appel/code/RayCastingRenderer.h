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

class PinholeCamera;

class RayCastingRenderer : public MyRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  Ray _generateShadowRay(const glm::vec3& point);

  glm::vec4 _renderPixel(float u, float v, MyScene* pScene, PinholeCamera* camera);

  // light
  glm::vec3 mLightPos = {-10, 10, 0};

};

}  // namespace RayTracingHistory
