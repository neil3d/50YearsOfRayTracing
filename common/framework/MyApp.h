#pragma once
#include <SDL2/SDL.h>

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
  MyRenderer::Ptr createRenderer() {
    if (!mMainWindow)
      throw MyException("MyApp.createRenderer: create window first.");

    mRenderer = std::make_shared<T>();
    mRenderer->_init(mMainWindow);
    return mRenderer;
  }

 private:
  void _tick();

 private:
  std::string mWindowTitle;
  MyRenderer::Ptr mRenderer;
  SDL_Window* mMainWindow = nullptr;

 public:
  MyApp(const MyApp&) = delete;
  MyApp& operator=(const MyApp&) = delete;
};