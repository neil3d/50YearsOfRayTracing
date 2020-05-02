#include "RayCastingRenderer.h"

#include <glm/glm.hpp>

void RayCastingRenderer::_init(SDL_Window* pWnd) {
  MyRenderer::_init(pWnd);
  _clearFrameBuffer(255, 255, 255, 255);
  _present();

  SDL_Delay(100);
  _asyncRender();
}

void RayCastingRenderer::_renderThread() {
  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (mRuning) {
        _drawSinglePixel(x, y);
      } else
        break;
    }  // end of for(x)
}

void RayCastingRenderer::_drawSinglePixel(int x, int y) {
  glm::vec4 color;

  _writePixel(x, y, color);
}