#pragma once
#include <SDL2/SDL.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "MyScene.h"

class MyRenderer {
 public:
  typedef std::shared_ptr<MyRenderer> Ptr;
  MyRenderer() = default;
  virtual ~MyRenderer() = default;

 public:
  float getProgress() const;

 public:
  virtual void _init(SDL_Window* pWnd);
  virtual void _present();
  virtual void _shutdown();

  void _renderThread();

 protected:
  void _writePixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void _clearFrameBuffer(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

 protected:
  int mFrameWidth = 0;
  int mFrameHeight = 0;

 private:
  SDL_Window* mWindow = nullptr;
  SDL_Surface* mSurface = nullptr;
  std::atomic<bool> mRuning = {true};

  std::mutex mMutex;
  std::atomic<uint32_t> mPixelCount = {0};
  std::vector<uint32_t> mFrameBuffer;

 public:
  MyRenderer(const MyRenderer&) = delete;
  MyRenderer& operator=(const MyRenderer&) = delete;
};