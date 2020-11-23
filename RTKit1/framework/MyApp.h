/**
 * 50 Years of Ray tracing
 *
 * GitHub: https://github.com/neil3d/50YearsOfRayTracing
 *
 * Created by yanliang.fyl, 2020
 */

#pragma once
#include <SDL2/SDL.h>

#include "MyException.h"
#include "MyRenderer.h"

namespace RTKit1 {
class MyApp {
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
  virtual void _onSDLEvent(const SDL_Event& evt) {}

  void _tick();
  void _showProgress();

 protected:
  std::string mWindowTitle;
  MyRenderer::Ptr mRenderer;
  SDL_Window* mMainWindow = nullptr;

 public:
  MyApp(const MyApp&) = delete;
  MyApp& operator=(const MyApp&) = delete;
};
}  // namespace RTKit1
