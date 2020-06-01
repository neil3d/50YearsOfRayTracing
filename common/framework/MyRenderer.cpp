#include "MyRenderer.h"

#include <spdlog/spdlog.h>

#include <algorithm>

#include "MyException.h"

namespace RayTracingHistory {

float MyRenderer::getProgress() const {
  int total = mFrameWidth * mFrameHeight;
  int count = mPixelCount;
  if (count == total) return 1.0f;
  return (float)count / (float)total;
}

void MyRenderer::_init(SDL_Window* pWnd) {
  mSurface = SDL_GetWindowSurface(pWnd);
  SDL_GetWindowSize(pWnd, &mFrameWidth, &mFrameHeight);

  int ret = SDL_LockSurface(mSurface);
  if (ret < 0) throw MyException("MyRenderer._init FAILED", SDL_GetError());

  uint8_t bytesPerPixel = mSurface->format->BytesPerPixel;
  if (bytesPerPixel != 4) {
    spdlog::warn("BytesPerPixel != 4");
  }
  mFrameBuffer.resize(mSurface->h * mSurface->pitch / bytesPerPixel);
  SDL_UnlockSurface(mSurface);
}

bool MyRenderer::nextPresentReady() const {
  int line = mPixelCount / mFrameWidth;
  return line > mPresentLine;
}

bool MyRenderer::isDone() const { return mPresentLine == mFrameHeight; }

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

  mPresentLine = mPixelCount / mFrameWidth;
}

void MyRenderer::_writePixel(int x, int y, glm::vec4 color, float gama) {
  if (x >= mFrameWidth || y >= mFrameHeight) {
    spdlog::error("bad pixel coordinates");
    return;
  }

  // gama
  if (gama != 1.0f) {
    color.r = std::powf(color.r, gama);
    color.g = std::powf(color.g, gama);
    color.b = std::powf(color.b, gama);
  }

  uint8_t r = 255.5f * std::clamp(color.r, 0.0f, 1.0f);
  uint8_t g = 255.5f * std::clamp(color.g, 0.0f, 1.0f);
  uint8_t b = 255.5f * std::clamp(color.b, 0.0f, 1.0f);
  uint8_t a = 255.5f * std::clamp(color.a, 0.0f, 1.0f);

  uint32_t index = y * mFrameWidth + x;
  uint32_t colorValue = SDL_MapRGBA(mSurface->format, r, g, b, a);

  std::lock_guard lock(mMutex);
  mFrameBuffer[index] = colorValue;
}

void MyRenderer::_clearFrameBuffer(const glm::vec4& colorf) {
  if (!mSurface) {
    spdlog::error("MyRenderer._clearFrameBuffer: INVALID surface.");
  }

  uint8_t r = 255.5f * colorf.r;
  uint8_t g = 255.5f * colorf.g;
  uint8_t b = 255.5f * colorf.b;
  uint8_t a = 255.5f * colorf.a;

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
}  // namespace RayTracingHistory
