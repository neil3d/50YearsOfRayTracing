#pragma once
#include <SDL2/SDL.h>

#include <chrono>

#include "MyException.h"
#include "MyRenderer.h"

class MyApp final {
 public:
  MyApp() = default;
  ~MyApp() = default;

  void init();
  void createWindow(int width, int height, const char* szTitle);
  void mainLoop();
  void shutdown();

  template <typename T>
  MyRenderer::Ptr createRenderer(int presentInterval) {
    mPresentInterval = presentInterval;
    if (!mMainWindow)
      throw MyException("MyApp.createRenderer: create window first.");

    mRenderer = std::make_shared<T>();
    mRenderer->_init(mMainWindow);
    return mRenderer;
  }

 private:
  static uint32_t presentCallback(uint32_t interval, void* param);

  void _tick();

 private:
  std::string mWindowTitle;
  MyRenderer::Ptr mRenderer;
  SDL_Window* mMainWindow = nullptr;
  int mPresentInterval = 2000;
  std::chrono::steady_clock::time_point mLastFrameTime;

 public:
  MyApp(const MyApp&) = delete;
  MyApp& operator=(const MyApp&) = delete;
};