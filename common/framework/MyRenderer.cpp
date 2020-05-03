#include "MyRenderer.h"

#include <spdlog/spdlog.h>

#include <algorithm>

#include "MyException.h"

void MyRenderer::renderScene(MyScene::Ptr scene, MyCamera::Ptr camera,
                             const glm::vec4& clearColor) {
  // stop current
  if (mRenderingThread.joinable()) {
    mRuning = false;
    mRenderingThread.join();
  }

  // start new
  mRuning = true;
  mPixelCount = 0;
  mPresentLine = 0;
  _clearFrameBuffer(clearColor);
  mRenderingThread = std::thread(
      [this, scene, camera] { this->_renderThread(scene, camera); });
}

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

void MyRenderer::_shutdown() {
  mRuning = false;
  if (mRenderingThread.joinable()) mRenderingThread.join();
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

void MyRenderer::_renderThread(MyScene::Ptr scene, MyCamera::Ptr camera) {
  glm::vec4 topColor(1.0f, 1, 1, 1);
  glm::vec4 bottomColor(0.5f, 0.7f, 1.0f, 1);

  for (int y = 0; y < mFrameHeight; y++)
    for (int x = 0; x < mFrameWidth; x++) {
      if (mRuning) {
        float r = (float)y / mFrameHeight;
        _writePixel(x, y, bottomColor * r + topColor * (1.0f - r));
      } else
        break;
    }  // end of for(x)
}

void MyRenderer::_writePixel(int x, int y, const glm::vec4& color) {
  if (x >= mFrameWidth || y >= mFrameHeight) {
    spdlog::error("bad pixel coordinates");
    return;
  }

  uint8_t r = 255.5f * color.r;
  uint8_t g = 255.5f * color.g;
  uint8_t b = 255.5f * color.b;
  uint8_t a = 255.5f * color.a;

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