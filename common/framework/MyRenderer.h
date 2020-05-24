/**
 * A Brief History of Ray tracing
 *
 * GitHub: https://github.com/neil3d/RayTracingHistory
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <SDL2/SDL.h>

#include <atomic>
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "../scene/MyScene.h"
#include "MyCamera.h"

namespace RayTracingHistory {

class MyRenderer {
 public:
  typedef std::shared_ptr<MyRenderer> Ptr;
  MyRenderer() = default;
  virtual ~MyRenderer() = default;

 public:
  void renderScene(MyScene::Ptr scene, MyCamera::Ptr camera,
                   const glm::vec4& clearColor);
  bool nextPresentReady() const;
  bool isDone() const;
  float getProgress() const;

 public:
  virtual void _init(SDL_Window* pWnd);
  virtual void _present();
  virtual void _shutdown();
  virtual void _renderThread(MyScene::Ptr scene, MyCamera::Ptr camera);

 protected:
  void _writePixel(int x, int y, const glm::vec4& color);
  void _clearFrameBuffer(const glm::vec4& color);

 protected:
  int mFrameWidth = 0;
  int mFrameHeight = 0;
  int mPresentLine = 0;

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
}  // namespace RayTracingHistory
