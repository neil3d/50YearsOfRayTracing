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

class PinholeCamera;

class RayCastingRenderer : public DirectRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  Ray _generateShadowRay(const glm::vec3& point);

  /* return degree of drakness*/
  float _castRay(float u, float v, MyScene* pScene, PinholeCamera* camera);

  void _drawDrakSign(int x, int y, float darkness);

  void _drawWireframe(MyScene* pScene, PinholeCamera* camera);

  // light
  glm::vec3 mLightPos = {-10, 20, 0};
};

}  // namespace RayTracingHistory
