#pragma once
#include "framework/MyRenderer.h"

class RayCastingRenderer : public MyRenderer {
 public:
  virtual void _renderThread(MyScene::Ptr scene) override;

 private:
  void _drawSinglePixel(int x, int y);
};