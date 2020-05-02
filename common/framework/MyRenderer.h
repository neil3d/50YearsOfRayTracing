#pragma once
#include <SDL2/SDL.h>

#include <atomic>
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "../scene/MyScene.h"

class MyRenderer {
 public:
  typedef std::shared_ptr<MyRenderer> Ptr;
  MyRenderer() = default;
  virtual ~MyRenderer() = default;

 public:
  void renderScene(MyScene::Ptr scene, const glm::vec4& clearColor);
  float getProgress() const;

 public:
  virtual void _init(SDL_Window* pWnd);
  virtual void _present();
  virtual void _shutdown();
  virtual void _renderThread(MyScene::Ptr scene);

 protected:
  void _writePixel(int x, int y, const glm::vec4& color);
  void _clearFrameBuffer(const glm::vec4& color);

 protected:
  int mFrameWidth = 0;
  int mFrameHeight = 0;

  SDL_Surface* mSurface = nullptr;
  std::atomic<bool> mRuning = {true};

  std::mutex mMutex;
  std::thread mRenderingThread;
  std::atomic<uint32_t> mPixelCount = {0};
  std::vector<uint32_t> mFrameBuffer;

 public:
  MyRenderer(const MyRenderer&) = delete;
  MyRenderer& operator=(const MyRenderer&) = delete;
};