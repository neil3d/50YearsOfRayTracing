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

    int w = 0, h = 0;
    SDL_GetWindowSize(mMainWindow, &w, &h);

    mRenderer = std::make_shared<T>();
    mRenderer->_init(w, h);
    return mRenderer;
  }

 private:
  void _render(SDL_Surface* pSurface);

 private:
  MyRenderer::Ptr mRenderer;
  SDL_Window* mMainWindow = nullptr;

 public:
  MyApp(const MyApp&) = delete;
  MyApp& operator=(const MyApp&) = delete;
};