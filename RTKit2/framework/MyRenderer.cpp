#include "MyRenderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <algorithm>
#include <iostream>

#include "MyException.h"

namespace RTKit2 {

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
    std::cerr << "BytesPerPixel != 4" << std::endl;
  }

  mFrameBuffer.resize(mSurface->h * mSurface->pitch / bytesPerPixel);
  SDL_UnlockSurface(mSurface);

  // log info
  std::cout << "SDL surface format = "
            << SDL_GetPixelFormatName(mSurface->format->format) << std::endl;
}

bool MyRenderer::nextPresentReady() const {
  uint32_t line = mPixelCount / mFrameWidth;
  return line > mPresentLine;
}

void MyRenderer::screenshot(const std::string& szFileName) {
  std::lock_guard lock(mMutex);

  // convert to RGB
  struct RGB {
    uint8_t R, G, B;
  };
  size_t count = mFrameBuffer.size();
  std::vector<RGB> buf(count);

  for (int i = 0; i < count; i++) {
    auto& out = buf[i];
    SDL_GetRGB(mFrameBuffer[i], mSurface->format, &out.R, &out.G, &out.B);
  }

  // write png
  int ret = stbi_write_png(szFileName.c_str(), mFrameWidth, mFrameHeight, 3,
                           buf.data(), mFrameWidth * sizeof(RGB));
  if (ret) {
    std::cout << "screenshot: " << SDL_GetBasePath() << szFileName << std::endl;
  } else {
    std::cerr << "screenshot FAILED!" << std::endl;
  }
}

void MyRenderer::renderScene(MyScene::Ptr scene, MyCamera::Ptr camera,
                             const glm::vec4& clearColor) {
  mStartupTime = std::chrono::steady_clock::now();
  mRuning = true;
  mFinished = false;
}

void MyRenderer::_present() {
  int ret = SDL_LockSurface(mSurface);
  if (ret < 0) {
    std::cerr << "SDL_LockSurface FAILED." << std::endl;
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

void MyRenderer::_onRenderFinished() {
  std::lock_guard lock(mMutex);
  auto finishTime = std::chrono::steady_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::duration<float>>(
      finishTime - mStartupTime);
  std::cout << "render finished in " << time.count() << "seconds." << std::endl;
  mFinished = true;
}

void MyRenderer::_writePixel(int x, int y, glm::vec4 color, float gama) {
  if (x >= mFrameWidth || y >= mFrameHeight) {
    std::cerr << "bad pixel coordinates" << std::endl;
    return;
  }

  // gama
  if (gama != 1.0f) {
    color.r = std::pow(color.r, gama);
    color.g = std::pow(color.g, gama);
    color.b = std::pow(color.b, gama);
  }

#if 1
  uint8_t r = uint8_t(255.5f * std::clamp(color.r, 0.0f, 1.0f));
  uint8_t g = uint8_t(255.5f * std::clamp(color.g, 0.0f, 1.0f));
  uint8_t b = uint8_t(255.5f * std::clamp(color.b, 0.0f, 1.0f));
  uint8_t a = uint8_t(255.5f * std::clamp(color.a, 0.0f, 1.0f));
#else
  uint8_t r = uint8_t(255.5f * color.r);
  uint8_t g = uint8_t(255.5f * color.g);
  uint8_t b = uint8_t(255.5f * color.b);
  uint8_t a = uint8_t(255.5f * color.a);
#endif

  uint32_t index = y * mFrameWidth + x;
  uint32_t colorValue = SDL_MapRGBA(mSurface->format, r, g, b, a);

  std::lock_guard lock(mMutex);
  mFrameBuffer[index] = colorValue;
}

void MyRenderer::_clearFrameBuffer(const glm::vec4& colorf) {
  if (!mSurface) {
    std::cerr << "MyRenderer._clearFrameBuffer: INVALID surface." << std::endl;
  }

  uint8_t r = uint8_t(255.5f * colorf.r);
  uint8_t g = uint8_t(255.5f * colorf.g);
  uint8_t b = uint8_t(255.5f * colorf.b);
  uint8_t a = uint8_t(255.5f * colorf.a);

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

}  // namespace RTKit2
