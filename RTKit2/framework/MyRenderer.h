/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <SDL2/SDL.h>
#include <embree3/rtcore.h>

#include <atomic>
#include <chrono>
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "../scene/MyScene.h"
#include "MyCamera.h"

namespace RTKit2 {

class MyScene;

class MyRenderer {
 public:
  typedef std::shared_ptr<MyRenderer> Ptr;
  MyRenderer() = default;
  virtual ~MyRenderer() = default;

 public:
  virtual void renderScene(MyScene::Ptr scene, MyCamera::Ptr camera,
                           const glm::vec4& clearColor);
  virtual bool isDone() const { return mFinished; }
  virtual float getProgress() const;
  virtual std::string getInfo() const { return ""; }

  virtual bool nextPresentReady() const;

  void screenshot(const std::string& szFileName);

 public:
  virtual void _init(SDL_Window* pWnd);
  virtual void _present();
  virtual void _shutdown() = 0;
  virtual void _onRenderFinished();

 protected:
  void _writePixel(int x, int y, glm::vec4 color, float gama);
  void _clearFrameBuffer(const glm::vec4& color);

 protected:
  int mFrameWidth = 0;
  int mFrameHeight = 0;

  std::chrono::steady_clock::time_point mStartupTime;

  SDL_Surface* mSurface = nullptr;
  std::atomic<bool> mRuning = {true};
  std::atomic<bool> mFinished = {false};

  std::mutex mMutex;
  std::atomic<uint32_t> mPresentLine = {0};
  std::atomic<uint32_t> mPixelCount = {0};
  std::vector<uint32_t> mFrameBuffer;

 public:
  MyRenderer(const MyRenderer&) = delete;
  MyRenderer& operator=(const MyRenderer&) = delete;
};
}  // namespace RTKit2
