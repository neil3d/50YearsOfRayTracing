#include "MyRenderer.h"

#include <spdlog/spdlog.h>

#include "MyException.h"

float MyRenderer::getProgress() const {
  float total = (float)mFrameWidth * mFrameHeight;
  return mPixelCount / total;
}

void MyRenderer::_init(SDL_Window* pWnd) {
  SDL_GetWindowSize(pWnd, &mFrameWidth, &mFrameHeight);
  mSurface = SDL_GetWindowSurface(pWnd);

  int ret = SDL_LockSurface(mSurface);
  if (ret < 0) throw MyException("SDL_LockSurface FAILED", SDL_GetError());

  mFrameBuffer.resize(mSurface->h * mSurface->pitch);
  SDL_UnlockSurface(mSurface);
}

void MyRenderer::_shutdown() {}

void MyRenderer::_present() {
  int ret = SDL_LockSurface(mSurface);
  if (ret < 0) {
    spdlog::error("SDL_LockSurface FAILED.");
    return;
  }

  // copy frame buffer to surface
  {
    std::lock_guard lock(mMutex);
    SDL_memcpy4(mSurface->pixels, mFrameBuffer.data(),
                mSurface->h * mSurface->pitch);
  }

  SDL_UnlockSurface(mSurface);
}

void MyRenderer::_writePixel(int x, int y, uint8_t r, uint8_t g, uint8_t b,
                             uint8_t a) {
  if (x >= mFrameWidth || y >= mFrameHeight) {
    spdlog::error("bad pixel coordinates");
    return;
  }

  mPixelCount++;
  uint32_t index = y * mFrameWidth + x;
  uint32_t color = SDL_MapRGBA(mSurface->format, r, g, b, a);

  std::lock_guard lock(mMutex);
  mFrameBuffer[index] = color;
}