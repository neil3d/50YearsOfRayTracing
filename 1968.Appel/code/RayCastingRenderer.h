#pragma once
#include "framework/MyRenderer.h"

class RayCastingRenderer : public MyRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) override;

 private:
  Ray _generateEyeRay(int x, int y);
  Ray _generateShadowRay(const glm::vec3& point);
  
  void _drawSinglePixel(int x, int y, MyScene* pScene);

  // light
  glm::vec3 mLightDir;

  // cache camera param
  glm::vec3 mEyePos;
  glm::vec3 mNearPlaneH, mNearPlaneV, mNearPlaneLeftBottom;
};