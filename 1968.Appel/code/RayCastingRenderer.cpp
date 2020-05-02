#include "RayCastingRenderer.h"

void RayCastingRenderer::_init(SDL_Window* pWnd) {
  MyRenderer::_init(pWnd);
  _clearFrameBuffer(255, 255, 255, 255);
  _present();
}
