#include "MyRenderer.h"

#include <spdlog/spdlog.h>

#include <algorithm>

#include "MyException.h"

float MyRenderer::getProgress() const {
  float total = (float)mFrameWidth * mFrameHeight;
  return mPixelCount / total;
}

void MyRenderer::_init(SDL_Window* pWnd) {
  mWindow = pWnd;
  SDL_GetWindowSize(pWnd, &mFrameWidth, &mFrameHeight);
  mSurface = SDL_GetWindowSurface(pWnd);

  int ret = SDL_LockSurface(mSurface);
  if (ret < 0) throw MyException("MyRenderer._init FAILED", SDL_GetError());

  uint8_t bytesPerPixel = mSurface->format->BytesPerPixel;
  if (bytesPerPixel != 4) {
    spdlog::warn("BytesPerPixel != 4");
  }
  mFrameBuffer.resize(mSurface->h * mSurface->pitch / bytesPerPixel);
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
    uint8_t bytesPerPixel = mSurface->format->BytesPerPixel;
    SDL_memcpy4(mSurface->pixels, mFrameBuffer.data(),
                mSurface->h * mSurface->pitch / bytesPerPixel);
  }

  SDL_UnlockSurface(mSurface);

  SDL_UpdateWindowSurface(mWindow);
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

void MyRenderer::_clearFrameBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  if (!mSurface) {
    spdlog::error("MyRenderer._clearFrameBuffer: INVALID surface.");
  }
  uint32_t color = SDL_MapRGBA(mSurface->format, r, g, b, a);
  {
    std::lock_guard lock(mMutex);
    std::fill(std::begin(mFrameBuffer), std::end(mFrameBuffer), color);
  }

  int ret = SDL_LockSurface(mSurface);
  if (ret >= 0) {
    uint8_t bytesPerPixel = mSurface->format->BytesPerPixel;
    SDL_memset4(mSurface->pixels, color,
                mSurface->h * mSurface->pitch / bytesPerPixel);
    SDL_UnlockSurface(mSurface);
  }
}