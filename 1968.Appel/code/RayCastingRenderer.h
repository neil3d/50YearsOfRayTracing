#pragma once
#include "framework/MyRenderer.h"

class RayCastingRenderer : public MyRenderer {
 public:
  virtual void _init(SDL_Window* pWnd) override;
  virtual void _renderThread() override;

 private:
  void _drawSinglePixel(int x, int y);
};